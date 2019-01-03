/*
 * nvm_beta_cmd - special nvm cmd api for Beta
 * (Beta - a code name for the special specification)
 *
 * Copyright (C) Xiaodong Liu <xiaodong.liu@intel.com>
 * All rights reserved.
 *
 * Contact author for license details.
 */

#include <liblightnvm.h>
#include <nvm_be.h>
#include <nvm_dev.h>
#include <nvm_beta_cmd.h>


static struct nvm_beta_spec_dev *beta_spec_devs[] = {
		&beta_apple_dev,
		NULL,
};

struct nvm_beta_spec_dev *nvm_beta_dev_recognition(struct nvm_dev *dev)
{
	int rc;

	for (int i = 0; beta_spec_devs[i]; i++) {
		rc = beta_spec_devs[i]->beta_dev_recognition(dev);
		if (rc == 1) {
			return beta_spec_devs[i];
		}
	}

	return NULL;
}

ssize_t nvm_addr_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_read) {
		fprintf(stderr, "addr_read not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_read(dev, addrs, naddrs, data, meta, flags, unclear, ret);
}

/* Find/Replace to extract func_p and parameters:

extended:
(
\n

regex:
.+nvm_(\w+)
\1

.+?\s{1}\*?([\w\[\]]+)[,)]
\1,



extended:
, \r\n
,

[]


, ;


 */
ssize_t nvm_addr_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  const void *data, const void *meta, uint16_t flags,
		  struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_write) {
		fprintf(stderr, "addr_write not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_write(dev, addrs, naddrs, data, meta, flags, ret);
}

ssize_t nvm_addr_erase(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_erase) {
		fprintf(stderr, "addr_erase not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_erase(dev, addrs, naddrs, flags, ret);
}

ssize_t nvm_addr_erase_sb(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_erase_sb) {
		fprintf(stderr, "addr_erase_sb not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_erase_sb(dev, addrs, naddrs, flags, ret);
}

ssize_t nvm_addr_parity_init(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_parity_init) {
		fprintf(stderr, "addr_parity_init not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_parity_init(dev, addrs, naddrs, ret);
}

ssize_t nvm_addr_parity_out(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_parity_out) {
		fprintf(stderr, "addr_parity_out not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_parity_out(dev, addrs, naddrs, ret);
}

ssize_t nvm_addr_protected_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		void *data, void *meta, uint16_t flags,
		int head_idx, int fake_write, int no_meta, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_protected_write) {
		fprintf(stderr, "addr_protected_write not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_protected_write(dev, addrs, naddrs, data, meta, flags, head_idx, fake_write, no_meta, ret);
}

ssize_t nvm_addr_first_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, uint16_t flags, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_first_read) {
		fprintf(stderr, "addr_first_read not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_first_read(dev, addrs, naddrs, data, flags, ret);
}

ssize_t nvm_addr_over_write_page(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 int head, struct nvm_ret *ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_over_write_page) {
		fprintf(stderr, "addr_over_write_page not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_over_write_page(dev, addrs, naddrs, head, ret);
}

ssize_t nvm_read_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->read_pm) {
		fprintf(stderr, "read_pm not implemented\n");
		return -1;
	}

	return dev->beta_dev->read_pm(dev, buf, flags, length, offset, ret);
}

ssize_t nvm_write_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                     unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->write_pm) {
		fprintf(stderr, "write_pm not implemented\n");
		return -1;
	}

	return dev->beta_dev->write_pm(dev, buf, flags, length, offset, ret);
}

ssize_t nvm_get_mef_log(struct nvm_dev* dev, struct nvm_log_page *log)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->get_mef_log) {
		fprintf(stderr, "get_mef_log not implemented\n");
		return -1;
	}

	return dev->beta_dev->get_mef_log(dev, log);
}

ssize_t nvm_get_mef_log(struct nvm_dev* dev, struct nvm_log_page *log);

ssize_t nvm_get_async_cmd_event(struct nvm_dev* dev, struct result_ctx* rctx)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->get_async_cmd_event) {
		fprintf(stderr, "get_async_cmd_event not implemented\n");
		return -1;
	}

	return dev->beta_dev->get_async_cmd_event(dev, rctx);
}

ssize_t nvm_addr_async_read(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_async_read) {
		fprintf(stderr, "addr_async_read not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_async_read(dev, ctx, flags, no_meta);
}

ssize_t nvm_addr_async_protected_write(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                                       int head_idx, int fake_write, int no_meta)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_async_protected_write) {
		fprintf(stderr, "addr_async_protected_write not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_async_protected_write(dev, ctx, flags, head_idx, fake_write, no_meta);
}

ssize_t nvm_addr_async_erase_sb(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_async_erase_sb) {
		fprintf(stderr, "addr_async_erase_sb not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_async_erase_sb(dev, ctx, flags);
}

ssize_t nvm_addr_async_parity_init(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_async_parity_init) {
		fprintf(stderr, "addr_async_parity_init not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_async_parity_init(dev, ctx);
}

ssize_t nvm_addr_async_parity_out(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->addr_async_parity_out) {
		fprintf(stderr, "addr_async_parity_out not implemented\n");
		return -1;
	}

	return dev->beta_dev->addr_async_parity_out(dev, ctx);
}

ssize_t nvm_async_read_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->async_read_pm) {
		fprintf(stderr, "async_read_pm not implemented\n");
		return -1;
	}

	return dev->beta_dev->async_read_pm(dev, ctx, flags, length, offset);
}

ssize_t nvm_async_write_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                    unsigned int length, unsigned int offset)
{
	if (dev->beta_dev == NULL) {
		return -1;
	}

	if (!dev->beta_dev->async_write_pm) {
		fprintf(stderr, "async_write_pm not implemented\n");
		return -1;
	}

	return dev->beta_dev->async_write_pm(dev, ctx, flags, length, offset);
}

