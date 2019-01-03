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
#include "nvm_be_spdk_advanced.h"
#include "spdk/ocssd_apple_dev_cmd.h"

#define BYPASS_UNSUPPORT	1

enum ctx_op {
	OP_WRITE = 0X91,
	OP_READ = 0X92,
	OP_PARITY = 0XA4,
};

#define APPLE_DEV_FAKE_LBA	0x1234ul
#define APPLE_DEV_PHY_RESET	0

static ssize_t nvm_addr_beta_apple_rw(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret, int read)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_nvme_cmd cmd = {0};
	size_t sector_nbytes = dev->geo.sector_nbytes;
	size_t meta_nbytes = dev->geo.meta_nbytes;
	void *data_loop, *meta_loop = NULL;
	int rc, i;
	uint64_t d_addr;

	for (i = 0; i < naddrs; i++) {
		data_loop = data + sector_nbytes * i;
		if (meta_nbytes != 0 && meta != NULL) {
			meta_loop = meta + meta_nbytes * i;
		}

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		if (read) {
			spdk_ocssd_apple_read_cmd(&cmd, d_addr, APPLE_DEV_FAKE_LBA);
		} else {
			spdk_ocssd_apple_write_cmd(&cmd, d_addr, APPLE_DEV_FAKE_LBA);
		}

		rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, data_loop, sector_nbytes,
				meta_loop, meta_nbytes, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_apple_read");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_apple_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret)
{
	return nvm_addr_beta_apple_rw(dev, addrs, naddrs, data, meta, flags, unclear, ret, 1);
}

ssize_t nvm_addr_beta_apple_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  const void *data, const void *meta, uint16_t flags,
		  struct nvm_ret *ret)
{
	return nvm_addr_beta_apple_rw(dev, addrs, naddrs, (void *)data, (void *)meta, flags, 0, ret, 0);
}

ssize_t nvm_addr_beta_apple_erase(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	int rc, i;
	uint64_t d_addr;

	for (i = 0; i < naddrs; i++) {

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		spdk_ocssd_apple_chunk_reset_cmd(&cmd, d_addr, APPLE_DEV_PHY_RESET);

		rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, NULL, 0,
				NULL, 0, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_apple_erase");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_apple_erase_sb(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	size_t npugrp = dev->geo.l.npugrp;
	size_t npunit = dev->geo.l.npunit;
	struct nvm_addr addr_tmp[1] = {0};
	int rc, i;
	uint16_t npugrp_i, npunit_i;
	uint64_t d_addr;

	for (i = 0; i < naddrs; i++) {
		if (addrs[i].g.ch != 0 || addrs[i].g.lun != 0 || addrs[i].g.pg != 0) {
			NVM_DEBUG("FAILED: illegal address for super block");
			errno = -EIO;
			return -1;
		}

		addr_tmp->g.blk = addrs[i].g.blk;
		for (npunit_i = 0; npunit_i < npunit; npunit_i++) {
			addr_tmp->g.lun = npunit_i;

			for (npugrp_i = 0; npugrp_i < npugrp; npugrp_i++) {
				addr_tmp->g.ch = npugrp_i;

				d_addr = nvm_be_addr_gen2dev_glue(dev,  addr_tmp);
				spdk_ocssd_apple_chunk_reset_cmd(&cmd, d_addr, APPLE_DEV_PHY_RESET);
				rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, NULL, 0,
						NULL, 0, &internal_ret);
				if (ret != NULL) {
					*ret = *(struct nvm_ret *)&internal_ret;
				}

				if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
					NVM_DEBUG("FAILED: nvm_addr_beta_apple_erase_sb");
					return -1;
				}
			}
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_apple_parity_init(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	int rc, i, j;
	uint64_t d_addr;
	uint64_t *chk_ppa_lists;
	struct nvm_addr addr_tmp;

	chk_ppa_lists = calloc(1, sizeof(uint64_t) * 8);

	for (i = 0; i < naddrs; i++) {
		addr_tmp = nvm_be_addr_format_glue(dev, &addrs[i]);
		//TODO: update this test
		for (j = 0; j < 7; j++) {
			addr_tmp.l.punit = 0;
			addr_tmp.l.pugrp = j;
			d_addr = nvm_be_addr_gen2dev_glue(dev,  &addr_tmp);
			chk_ppa_lists[j] = d_addr;
		}
		addr_tmp.l.punit = 0;
		addr_tmp.l.pugrp = j;
		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addr_tmp);

		spdk_ocssd_apple_parity_init_cmd(&cmd, (uint64_t)chk_ppa_lists, 8, d_addr);

		rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, NULL, 0,
				NULL, 0, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_apple_parity_init");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_apple_parity_out(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs, struct nvm_ret *ret)
{
	return -1;
}

ssize_t nvm_addr_beta_apple_protected_write(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		void *data, void *meta, uint16_t flags,
		int head_idx, int fake_write, int no_meta, struct nvm_ret *ret)
{
	return nvm_addr_beta_apple_rw(dev, addrs, naddrs, (void *)data, (void *)meta, flags, 0, ret, 0);
}

ssize_t nvm_addr_beta_apple_first_read(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, uint16_t flags, struct nvm_ret *ret)
{
	return -1;
}

ssize_t nvm_addr_beta_apple_over_write_page(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 int head, struct nvm_ret *ret)
{
	return -1;
}

ssize_t nvm_beta_apple_read_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                 unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	return -1;
}

ssize_t nvm_beta_apple_write_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                 unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	return -1;
}

ssize_t nvm_beta_apple_get_mef_log(struct nvm_dev* dev, struct nvm_log_page *log)
{
	return -1;
}


struct cmd_ctx_naddr_arg {
	struct cmd_ctx* ctx;
	int cpl_count;
};
ssize_t nvm_beta_apple_get_async_cmd_event(struct nvm_dev* dev, struct result_ctx* rctx)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	int nr_min = rctx->max_count;
	int nr = RESULT_MAX_CMD_CTX; // from the struct definition of result ctx;
	struct spdk_bdev_aio_req *reqs[RESULT_MAX_CMD_CTX];
	int rc, i, j;

	assert(async_ctx);

	rctx->max_count = 0;
	for (i = 0; i < nr_min - rctx->max_count; i++) {
		rc = spdk_bdev_aio_ctx_get_reqs(async_ctx,
				nr_min, nr, reqs, NULL);
		if (rc < 0) {
			errno = rc;
			return -1;
		} else if (rc == 0){
			break;
		}

		for (j = 0; j < rc; j++) {
			naddr_arg = spdk_bdev_aio_req_get_private_arg(reqs[j]);

			/* Set result and status */
			internal_ret = *spdk_bdev_aio_req_get_ret(reqs[j]);
			if (spdk_bdev_aio_ret_check(&internal_ret)) {
				naddr_arg->ctx->status = internal_ret.status;
				naddr_arg->ctx->result = internal_ret.result;
			}

			/* Filter out the ctx for multi-nvm-addr */
			assert(naddr_arg->cpl_count > 0);
			naddr_arg->cpl_count--;
			if (naddr_arg->cpl_count == 0) {
				/* cmd_ctx is finished */
				rctx->ctx[rctx->max_count] = naddr_arg->ctx;
				rctx->max_count++;
				free(naddr_arg);
			}

			free(reqs[j]);
		}
	}

	return 0;
}

//TODO: some flags examples?
ssize_t nvm_addr_beta_apple_async_read(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct spdk_nvme_cmd cmd = {0};
	int naddrs = ctx->naddrs;
	struct nvm_addr *addrs = ctx->addrs;
	int rc, i;
	size_t sector_nbytes = dev->geo.l.nbytes;
	size_t meta_nbytes = dev->geo.l.nbytes_oob;
	void *data = ctx->data, *meta = ctx->meta;
	void *data_loop, *meta_loop = NULL;
	struct spdk_bdev_aio_req **reqs = NULL;
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	uint64_t d_addr;

	assert(async_ctx);

	naddr_arg = malloc(sizeof(*naddr_arg));
	if (naddr_arg == NULL) {
		errno = -ENOMEM;
		return -1;
	}
	naddr_arg->cpl_count = naddrs;
	naddr_arg->ctx = ctx;

	reqs = calloc(naddrs, sizeof(*reqs));
	if (reqs == NULL) {
		errno = -ENOMEM;
		goto failed;
	}
	for (i = 0; i < naddrs; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = -ENOMEM;
			goto failed;
		}
	}

	for (i = 0; i < naddrs; i++) {
		data_loop = data + sector_nbytes * i;
		if (meta_nbytes != 0 && meta != NULL) {
			meta_loop = meta + meta_nbytes * i;
		}

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		spdk_ocssd_apple_read_cmd(&cmd, d_addr, APPLE_DEV_FAKE_LBA);

		spdk_bdev_aio_req_set_io_passthru(reqs[i], &cmd, data_loop, sector_nbytes,
				meta_loop, meta_nbytes);
		spdk_bdev_aio_req_set_private_arg(reqs[i], naddr_arg);
	}

	ctx->opcode = OP_READ;

	rc = spdk_bdev_aio_ctx_submit(async_ctx, naddrs, reqs);
	if (rc) {
		goto failed;
	}

	free(reqs);
	return 0;

failed:
	if (reqs != NULL) {
		for (i = 0; i < naddrs; i++) {
			if (reqs[i] != NULL) {
				free(reqs[i]);
			}
		}
	}

	free(naddr_arg);
	free(reqs);

	return -1;
}


ssize_t nvm_addr_beta_apple_async_protected_write(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                                   int head_idx, int fake_write, int no_meta)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct spdk_nvme_cmd cmd = {0};
	int naddrs = ctx->naddrs;
	struct nvm_addr *addrs = ctx->addrs;
	int rc, i;
	size_t sector_nbytes = dev->geo.l.nbytes;
	size_t meta_nbytes = dev->geo.l.nbytes_oob;
	void *data = ctx->data, *meta = ctx->meta;
	void *data_loop, *meta_loop = NULL;
	struct spdk_bdev_aio_req **reqs = NULL;
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	uint64_t d_addr;

	assert(async_ctx);

	naddr_arg = malloc(sizeof(*naddr_arg));
	if (naddr_arg == NULL) {
		errno = -ENOMEM;
		return -1;
	}
	naddr_arg->cpl_count = naddrs;
	naddr_arg->ctx = ctx;

	reqs = calloc(naddrs, sizeof(*reqs));
	if (reqs == NULL) {
		errno = -ENOMEM;
		goto failed;
	}
	for (i = 0; i < naddrs; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = -ENOMEM;
			goto failed;
		}
	}

	for (i = 0; i < naddrs; i++) {
		data_loop = data + sector_nbytes * i;
		if (meta_nbytes != 0 && meta != NULL) {
			meta_loop = meta + meta_nbytes * i;
		}

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		spdk_ocssd_apple_write_cmd(&cmd, d_addr, APPLE_DEV_FAKE_LBA);

		spdk_bdev_aio_req_set_io_passthru(reqs[i], &cmd, data_loop, sector_nbytes,
				meta_loop, meta_nbytes);
		spdk_bdev_aio_req_set_private_arg(reqs[i], naddr_arg);
	}

	ctx->opcode = OP_WRITE;

	rc = spdk_bdev_aio_ctx_submit(async_ctx, naddrs, reqs);
	if (rc) {
		goto failed;
	}

	free(reqs);
	return 0;

failed:
	if (reqs != NULL) {
		for (i = 0; i < naddrs; i++) {
			if (reqs[i] != NULL) {
				free(reqs[i]);
			}
		}
	}

	free(naddr_arg);
	free(reqs);

	return -1;
}

ssize_t nvm_addr_beta_apple_async_erase_sb(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct spdk_nvme_cmd cmd = {0};
	int naddrs = ctx->naddrs;
	size_t npugrp = dev->geo.l.npugrp;
	size_t npunit = dev->geo.l.npunit;
	int nchks = npugrp * npunit;
	struct nvm_addr *addrs = ctx->addrs;
	int rc, i, req_idx;
	uint16_t npugrp_i, npunit_i;
	struct spdk_bdev_aio_req **reqs = NULL;
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	uint64_t d_addr;
	struct nvm_addr addr_tmp[1] = {0};

	assert(async_ctx);

	naddr_arg = malloc(sizeof(*naddr_arg));
	if (naddr_arg == NULL) {
		errno = -ENOMEM;
		return -1;
	}
	naddr_arg->cpl_count = naddrs * nchks;
	naddr_arg->ctx = ctx;

	reqs = calloc(naddrs * nchks, sizeof(*reqs));
	if (reqs == NULL) {
		errno = -ENOMEM;
		goto failed;
	}
	for (i = 0; i < naddrs * nchks; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = -ENOMEM;
			goto failed;
		}
	}

	req_idx = 0;
	for (i = 0; i < naddrs; i++) {
		if (addrs[i].g.ch != 0 || addrs[i].g.lun != 0 || addrs[i].g.pg != 0) {
			NVM_DEBUG("FAILED: illegal address for super block");
			errno = -EIO;
			return -1;
		}

		addr_tmp->g.blk = addrs[i].g.blk;

		for (npunit_i = 0; npunit_i < npunit; npunit_i++) {
			addr_tmp->g.lun = npunit_i;

			for (npugrp_i = 0; npugrp_i < npugrp; npugrp_i++) {
				addr_tmp->g.ch = npugrp_i;

				d_addr = nvm_be_addr_gen2dev_glue(dev,  addr_tmp);
				spdk_ocssd_apple_chunk_reset_cmd(&cmd, d_addr, APPLE_DEV_PHY_RESET);

				spdk_bdev_aio_req_set_io_passthru(reqs[req_idx], &cmd, NULL, 0, NULL, 0);
				spdk_bdev_aio_req_set_private_arg(reqs[req_idx], naddr_arg);
				req_idx++;
			}
		}
	}

	ctx->opcode = OP_WRITE;

	rc = spdk_bdev_aio_ctx_submit(async_ctx, req_idx, reqs);
	if (rc) {
		goto failed;
	}

	free(reqs);
	return 0;

failed:
	if (reqs != NULL) {
		for (i = 0; i < naddrs * nchks; i++) {
			if (reqs[i] != NULL) {
				free(reqs[i]);
			}
		}
	}

	free(naddr_arg);
	free(reqs);

	return -1;
}

ssize_t nvm_addr_beta_apple_async_parity_init(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	return -1;
}
ssize_t nvm_addr_beta_apple_async_parity_out(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	return -1;
}

ssize_t nvm_beta_apple_async_read_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                unsigned int length, unsigned int offset)
{
	return -1;
}
ssize_t nvm_beta_apple_async_write_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                unsigned int length, unsigned int offset)
{
	return -1;
}

/* internal admin basic functions for backend */
struct nvm_spec_rprt *nvm_beta_apple_rprt(struct nvm_dev *dev, struct nvm_addr *addr, int opt,
			      struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_nvme_cmd cmd = {0};
	const size_t DESCR_NBYTES = sizeof(struct nvm_spec_rprt_descr);
	const struct nvm_geo *geo = nvm_dev_get_geo(dev);
	size_t lpo_off;;
	struct nvm_spec_rprt *rprt = NULL;
	struct nvm_spec_rprt_descr *chunkinfo_buf = NULL;
	size_t rprt_len, ndescr;
	int rc;

	if (NVM_SPEC_VERID_20 != dev->verid) {
		errno = EINVAL;
		return NULL;
	}

	lpo_off = addr ? nvm_addr_gen2lpo(dev, nvm_be_addr_format_glue(dev, addr)) : 0;
	ndescr = addr ? geo->l.nchunk : geo->l.nchunk * geo->l.npunit * geo->l.npugrp;
	rprt_len = ndescr * DESCR_NBYTES + sizeof(rprt->ndescr);

	rprt = malloc(rprt_len);
	chunkinfo_buf = nvm_buf_alloc(dev, ndescr * DESCR_NBYTES, NULL);
	if (!rprt || !chunkinfo_buf) {
		errno = ENOMEM;
		goto failed;
	}
	memset(rprt, 0, rprt_len);
	memset(chunkinfo_buf, 0, ndescr * DESCR_NBYTES);
	rprt->ndescr = ndescr;

	for (size_t i = 0; i < ndescr; i += 0x1000) {
		const size_t count = NVM_MIN(0x1000, ndescr - i);
		lpo_off += i * DESCR_NBYTES;

		spdk_ocssd_apple_chunkinfo_cmd(&cmd, lpo_off, count);

		rc = spdk_bdev_req_admin_passthru_sync(bt, (struct spdk_nvme_cmd *)&cmd, &chunkinfo_buf[i],
				count * DESCR_NBYTES, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc|| spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_be_spdk_advanced_geometry");
			goto failed;
		}
	}

	memcpy(rprt->descr, chunkinfo_buf, ndescr * DESCR_NBYTES);
	nvm_buf_free(dev, chunkinfo_buf);

	return rprt;


failed:
	free(rprt);
	nvm_buf_free(dev, chunkinfo_buf);
	return NULL;
}

int nvm_beta_apple_gfeat(struct nvm_dev *dev, uint8_t id, union nvm_nvme_feat *feat,
	     struct nvm_ret *ret)
{
	return -1;
}

int nvm_beta_apple_sfeat(struct nvm_dev *dev, uint8_t id, const union nvm_nvme_feat *feat,
	     struct nvm_ret *ret)
{
	return -1;
}

struct nvm_spec_bbt *nvm_beta_apple_gbbt(struct nvm_dev *dev, struct nvm_addr addr,
			     struct nvm_ret *ret)
{
	const uint32_t nblks = dev->geo.g.nblocks * dev->geo.g.nplanes;
	struct nvm_spec_bbt *bbt = NULL;
	const size_t bbt_len = sizeof(*bbt) + sizeof(*(bbt->blk)) * nblks;

	bbt = malloc(bbt_len);
	if (!bbt) {
		errno = ENOMEM;
		return NULL;
	}
	memset(bbt, 0, sizeof(*bbt));

	bbt->tblid[0] = 'B';
	bbt->tblid[1] = 'B';
	bbt->tblid[2] = 'L';
	bbt->tblid[3] = 'T';
	bbt->tblks = nblks;
	bbt->tfact = 0;
	bbt->tgrown = 0;
	bbt->tdresv = 0;
	bbt->thresv = 0;

	return bbt;
}

int nvm_beta_apple_sbbt(struct nvm_dev *dev, struct nvm_addr *addrs, int naddrs, uint16_t flags,
	    struct nvm_ret *ret)
{
	return -1;
}

static int beta_apple_dev_recognition(struct nvm_dev *dev)
{
	//TODO: add recognization

	return 1;
}

struct nvm_beta_spec_dev beta_apple_dev = {
		.beta_dev_recognition	= beta_apple_dev_recognition,

		.addr_read = nvm_addr_beta_apple_read,
		.addr_write = nvm_addr_beta_apple_write,
		.addr_erase = nvm_addr_beta_apple_erase,
		.addr_erase_sb = nvm_addr_beta_apple_erase_sb,
//		.addr_parity_init = nvm_addr_beta_apple_parity_init,
//		.addr_parity_out = nvm_addr_beta_apple_parity_out,
//		.addr_protected_write = nvm_addr_beta_apple_protected_write,
//		.addr_first_read = nvm_addr_beta_apple_first_read,
//		.addr_over_write_page = nvm_addr_beta_apple_over_write_page,
//		.read_pm = nvm_beta_apple_read_pm,
//		.write_pm = nvm_beta_apple_write_pm,
//		.get_mef_log = nvm_beta_apple_get_mef_log,

		.get_async_cmd_event = nvm_beta_apple_get_async_cmd_event,
		.addr_async_read = nvm_addr_beta_apple_async_read,
		.addr_async_protected_write = nvm_addr_beta_apple_async_protected_write,
		.addr_async_erase_sb = nvm_addr_beta_apple_async_erase_sb,
//		.addr_async_parity_init = nvm_addr_beta_apple_async_parity_init,
//		.addr_async_parity_out = nvm_addr_beta_apple_async_parity_out,
//		.async_read_pm = nvm_beta_apple_async_read_pm,
//		.async_write_pm = nvm_beta_apple_async_write_pm,

		.rprt = nvm_beta_apple_rprt,
		.gfeat = nvm_beta_apple_gfeat,
		.sfeat = nvm_beta_apple_sfeat,
		.gbbt = nvm_beta_apple_gbbt,
		.sbbt = nvm_beta_apple_sbbt,
};


