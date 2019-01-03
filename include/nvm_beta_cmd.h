/*
 * nvm_beta_cmd - special nvm cmd api for Beta
 * (Beta - a code name for the special specification)
 *
 * Copyright (C) Xiaodong Liu <xiaodong.liu@intel.com>
 * All rights reserved.
 *
 * Contact author for license details.
 */
#ifndef __NVM_BETA_CMD_H
#define __NVM_BETA_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <liblightnvm.h>
#include <errno.h>

/* sync mode cmd api for beta spec */

//TODO: clarify unclear parameter
ssize_t nvm_addr_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret);

ssize_t nvm_addr_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  const void *data, const void *meta, uint16_t flags,
		  struct nvm_ret *ret);

ssize_t nvm_addr_erase(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret);

ssize_t nvm_addr_erase_sb(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret);

ssize_t nvm_addr_parity_init(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret);

ssize_t nvm_addr_parity_out(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret);

ssize_t nvm_addr_protected_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		void *data, void *meta, uint16_t flags,
		int head_idx, int fake_write, int no_meta, struct nvm_ret *ret);

ssize_t nvm_addr_first_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, uint16_t flags, struct nvm_ret *ret);

ssize_t nvm_addr_over_write_page(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 int head, struct nvm_ret *ret);

ssize_t nvm_read_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret);

ssize_t nvm_write_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret);

//TODO: clarify definition of mef log
struct nvm_log_page;
ssize_t nvm_get_mef_log(struct nvm_dev* dev, struct nvm_log_page *log);

//TODO: add more nvm sync API

/* async mode cmd api for beta spec */
struct cmd_ctx {
    int naddrs;
    struct nvm_addr* addrs;
    void* data;
    void* meta;
    void* private_data;	/* user specific pointer */

    uint8_t opcode;	/* It is set after get_events. Maybe set in async read/write function */
    uint8_t flags;
    uint16_t control;
    uint16_t nppas;
    uint16_t rsvd;
    uint64_t metadata;
    uint64_t addr;
    uint64_t ppa_list;
    uint32_t metadata_len;
    uint32_t data_len;
    uint64_t status;	/* set by get_events */
    uint32_t result;	/* set by get_events */
    uint32_t rsvd3[3];
};

//TODO: why max_count? seems caller will set it to indicate the minimum events to wait; get_events will set it to indicate how many events are finished.
struct result_ctx {
    int max_count;
    struct cmd_ctx* ctx[20];
};

#define RESULT_MAX_CMD_CTX 20

/**
 * Handle complete command use async mode
 *
 * @note
 * handle complete command and return result to user
 *
 * @param dev Device handle obtained with `nvm_dev_open`
 * @param rctx query result context
 * @returns 0 on success. On error: returns -1, sets `errno` accordingly, and
 */
ssize_t nvm_get_async_cmd_event(struct nvm_dev* dev, struct result_ctx* rctx);

/**
 * Read content of nvm at addresses into buf use async mode
 *
 * @note
 * The addresses given to this function are interpreted as sector addresses, in
 * contrast to `nvm_addr_mark` and `nvm_addr_erase` for which the address is
 * interpreted as a block address.
 *
 * @param dev Device handle obtained with `nvm_dev_open`
 * @param ctx cmd context
 * @param flags Access mode
 * @param no_meta Whether read with metadata
 * @returns 0 on success. On error: returns -1, sets `errno` accordingly
 */
ssize_t nvm_addr_async_read(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta);

/**
 * Write content of buf to nvm at address(es) with RAID5 protection use async mode
 *
 * @note
 * This is much like nvm_addr_protected_write, except for async mode.
 *
 * @param head_idx Open block(head) index, two heads (0, 1) are supported. Set this
 *         field to 2 if you don't want RAID5 protection.
 * @param fake_write Write data to RAID engine only. No data is written to media
 * @param no_meta Whether write with metadata
 * @returns 0 on success. On error: returns -1, sets `errno` accordingly
 */
ssize_t nvm_addr_async_protected_write(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                                       int head_idx, int fake_write, int no_meta);

ssize_t nvm_addr_async_erase_sb(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags);

ssize_t nvm_addr_async_parity_init(struct nvm_dev* dev, struct cmd_ctx* ctx);

ssize_t nvm_addr_async_parity_out(struct nvm_dev* dev, struct cmd_ctx* ctx);

ssize_t nvm_async_read_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset);

ssize_t nvm_async_write_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset);

struct nvm_beta_spec_dev {
	/* device recognition, return 1 if recognized */
	int (* beta_dev_recognition)(struct nvm_dev *dev);

	/* synchronous operation */
	ssize_t (* addr_read)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			 void *data, void *meta, uint16_t flags, int unclear,
			 struct nvm_ret *ret);

	ssize_t (* addr_write)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			  const void *data, const void *meta, uint16_t flags,
			  struct nvm_ret *ret);

	ssize_t (* addr_erase)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			  uint16_t flags, struct nvm_ret *ret);

	ssize_t (* addr_erase_sb)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			  uint16_t flags, struct nvm_ret *ret);

	ssize_t (* addr_parity_init)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret);

	ssize_t (* addr_parity_out)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret);


	ssize_t (* addr_protected_write)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			void *data, void *meta, uint16_t flags,
			int head_idx, int fake_write, int no_meta, struct nvm_ret *ret);

	ssize_t (* addr_first_read)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			 void *data, uint16_t flags, struct nvm_ret *ret);

	ssize_t (* addr_over_write_page)(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
			 int head, struct nvm_ret *ret);

	ssize_t (* read_pm)(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret);

	ssize_t (* write_pm)(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret);

	ssize_t (* get_mef_log)(struct nvm_dev* dev, struct nvm_log_page *log);

	/* asynchronous operation */
	ssize_t (* get_async_cmd_event)(struct nvm_dev* dev, struct result_ctx* rctx);

	ssize_t (* addr_async_read)(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta);

	ssize_t (* addr_async_protected_write)(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                                       int head_idx, int fake_write, int no_meta);

	ssize_t (* addr_async_erase_sb)(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags);

	ssize_t (* addr_async_parity_init)(struct nvm_dev* dev, struct cmd_ctx* ctx);
	ssize_t (* addr_async_parity_out)(struct nvm_dev* dev, struct cmd_ctx* ctx);

	ssize_t (* async_read_pm)(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset);
	ssize_t (* async_write_pm)(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset);

	/* internal admin basic functions for backend */
	/**
	 * Execute report chunk command
	 */
	struct nvm_spec_rprt *(*rprt)(struct nvm_dev *, struct nvm_addr *, int,
				      struct nvm_ret *);

	/**
	 * Execute get feature command
	 */
	int (*gfeat)(struct nvm_dev *, uint8_t, union nvm_nvme_feat *,
		     struct nvm_ret *);

	/**
	 * Execute set feature command
	 */
	int (*sfeat)(struct nvm_dev *, uint8_t, const union nvm_nvme_feat *,
		     struct nvm_ret *);

	/**
	 * Execute get bad-block-table command
	 */
	struct nvm_spec_bbt *(*gbbt)(struct nvm_dev *, struct nvm_addr,
				     struct nvm_ret *);

	/**
	 * Execute set bad-block-table command
	 */
	int (*sbbt)(struct nvm_dev *, struct nvm_addr *, int, uint16_t,
		    struct nvm_ret *);
};

/* recognize what beta dev it is */
struct nvm_beta_spec_dev *nvm_beta_dev_recognition(struct nvm_dev *dev);

extern struct nvm_beta_spec_dev beta_apple_dev;

#ifdef __cplusplus
}
#endif

#endif /* __NVM_BETA_CMD_H */
