/*
 * nvm_beta_cmd - special nvm cmd api for Beta
 * (Beta - a code name for the special specification)
 *
 * Copyright (C) Xiaodong Liu <xiaodong.liu@intel.com>
 * All rights reserved.
 *
 * Contact author for license details.
 */

#include <assert.h>
#include <liblightnvm.h>
#include <nvm_be.h>
#include <nvm_dev.h>
#include <nvm_vblk.h>
#include <nvm_beta_cmd.h>
#include "nvm_be_spdk_advanced.h"
#include "spdk/ocssd_banana_dev_cmd.h"

#define BYPASS_UNSUPPORT	1

//#define DEBUG_DETAIL
#ifdef DEBUG_DETAIL
#define NVM_DEBUG_DETAIL	NVM_DEBUG
#else
#define NVM_DEBUG_DETAIL(...)
#endif

enum ctx_op {
	OP_WRITE = 0X91,
	OP_READ = 0X92,
	OP_PARITY_INIT = 0XA0,
	OP_PARITY_OUT = 0XA4,
};

#define BANANA_DEV_FAKE_LBA	0x1234ul
#define BANANA_DEV_PHY_RESET	0
#define BANANA_DEV_OPT_WRT_NSECTOR	32 // 128KB / 4KB

static int beta_banana_dev_chunk_is_offline(struct nvm_dev *dev, struct nvm_addr *addr_20);
static uint8_t beta_banana_dev_get_chunk_state(struct nvm_dev *dev, struct nvm_addr *addr_20);
static uint64_t beta_banana_dev_get_write_pointer(struct nvm_dev *dev, struct nvm_addr *addr_20);
static struct nvm_vblk *nvm_vblk_alloc_filtered_line(struct nvm_dev *dev, int chunk);
static void beta_banana_dev_vblk_set(struct nvm_dev *dev, int chunk, struct nvm_vblk *vblk);
static struct nvm_vblk *beta_banana_dev_vblk_get(struct nvm_dev *dev, int chunk_idx);


struct nvm_addr nvm_be_addr_format_glue(struct nvm_dev *dev, struct nvm_addr *addr_12)
{
	assert(dev->verid == NVM_SPEC_VERID_20);
	struct nvm_geo *geo = &dev->geo;
	struct nvm_addr addr_20 = {0};

	addr_20.l.pugrp = addr_12->g.ch;
	addr_20.l.punit = addr_12->g.lun;
	addr_20.l.chunk = geo->nblocks * addr_12->g.pl + addr_12->g.blk;
	addr_20.l.sectr = geo->nsectors * addr_12->g.pg + addr_12->g.sec;

	NVM_DEBUG_DETAIL("format1.2: ch=%d lun=%d blk=%d page=%d plane=%d sector=%d\n",
			addr_12->g.ch, addr_12->g.lun, addr_12->g.blk, addr_12->g.pg, addr_12->g.pl, addr_12->g.sec);
	NVM_DEBUG_DETAIL("format2: grp=%d punit=%d chunk=%d sector=%d\n",
			addr_20.l.pugrp, addr_20.l.punit, addr_20.l.chunk, addr_20.l.sectr);

	return addr_20;
}

uint64_t nvm_be_addr_gen2dev_glue(struct nvm_dev *dev, struct nvm_addr *addr_12)
{
	uint64_t d_addr = 0;
	struct nvm_addr addr_20;

	addr_20 = nvm_be_addr_format_glue(dev, addr_12);
	d_addr = nvm_addr_gen2dev(dev, addr_20);

	return d_addr;
}

static ssize_t nvm_addr_beta_banana_error_injection(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 int err_type, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_nvme_cmd cmd = {0};
	int rc, i;
	uint64_t d_addr;
	int ppa_matching_die = 0;

	/* Write error is aiming to the whole die */
	if (err_type == BANANA_PROGRAM_FAILURE_WITHOUT_READ) {
		ppa_matching_die = 1;
	}

	for (i = 0; i < naddrs; i++) {

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs_12[i]);
		spdk_ocssd_banana_error_injection(&cmd, err_type, ppa_matching_die, d_addr);

		rc = spdk_bdev_req_admin_passthru_sync(bt, &cmd, NULL, 0,
				&internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_banana_error_injection");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_banana_inject_read_failed(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_error_injection(dev, addrs_12, naddrs, BANANA_READ_FAILURE_AS_UNCORRECTABLE, ret);
}

ssize_t nvm_addr_beta_banana_inject_write_failed(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_error_injection(dev, addrs_12, naddrs, BANANA_PROGRAM_FAILURE_WITHOUT_READ, ret);
}

ssize_t nvm_addr_beta_banana_inject_erase_failed(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_error_injection(dev, addrs_12, naddrs, BANANA_ERASE_FAILURE, ret);
}

static ssize_t nvm_addr_beta_banana_error_injection_pending(struct nvm_dev *dev, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_nvme_cmd cmd = {0};
	int rc;


	spdk_ocssd_banana_error_injection_pending(&cmd);
	rc = spdk_bdev_req_admin_passthru_sync(bt, &cmd, NULL, 0,
			&internal_ret);
	if (ret != NULL) {
		*ret = *(struct nvm_ret *)&internal_ret;
	}

	if (rc) {
		NVM_DEBUG("FAILED: nvm_addr_beta_banana_error_injection_pending");
		return -1;
	}

	if (internal_ret.status == 0) {
		NVM_DEBUG("Error injection is not pending");
		return 0; // Error injection is not pending.
	} else if (internal_ret.status == 1) {
		NVM_DEBUG("Error injection is pending");
		return 1; // Error injection is pending.
	} else {
		NVM_DEBUG("FAILED: nvm_addr_beta_banana_error_injection_pending unknown status %lu", internal_ret.status);
		return -1;
	}
}

ssize_t nvm_beta_banana_clean_error_inject(struct nvm_dev *dev, struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_error_injection_pending(dev, ret);
}

ssize_t nvm_beta_banana_clean_read_error_inject(struct nvm_dev *dev)
{
	return nvm_addr_beta_banana_error_injection_pending(dev, NULL);
}


static ssize_t nvm_addr_beta_banana_rw(struct nvm_dev *dev, struct nvm_addr addrs[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret, int read, int is_addr_20)
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

		if (is_addr_20) {
			d_addr = nvm_addr_gen2dev(dev, addrs[i]);
		} else {
			d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		}

		if (read) {
			spdk_ocssd_banana_read_cmd(&cmd, d_addr, BANANA_DEV_FAKE_LBA);
		} else {
			spdk_ocssd_banana_write_cmd(&cmd, d_addr, BANANA_DEV_FAKE_LBA);
		}

		rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, data_loop, sector_nbytes,
				meta_loop, meta_nbytes, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_banana_rw");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_banana_read(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 void *data, void *meta, uint16_t flags, int unclear,
		 struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_rw(dev, addrs_12, naddrs, data, meta, flags, unclear, ret, 1, 0);
}

ssize_t nvm_addr_beta_banana_write(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		  const void *data, const void *meta, uint16_t flags,
		  struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_rw(dev, addrs_12, naddrs, (void *)data, (void *)meta, flags, 0, ret, 0, 0);
}

ssize_t nvm_addr_beta_banana_erase(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	int rc, i;
	uint64_t d_addr;

	for (i = 0; i < naddrs; i++) {

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs_12[i]);
		spdk_ocssd_banana_chunk_reset_cmd(&cmd, d_addr, BANANA_DEV_PHY_RESET);

		rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, NULL, 0,
				NULL, 0, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_addr_beta_banana_erase");
			return -1;
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_banana_erase_one_sb(struct nvm_dev *dev, struct nvm_addr *addr_12,
		  uint16_t flags, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	size_t npugrp = dev->geo.l.npugrp;
	size_t npunit = dev->geo.l.npunit;
	struct nvm_addr addr_20_tmp[1] = {0};
	struct nvm_addr addr_20;
	int rc;
	uint16_t npugrp_i, npunit_i;
	uint64_t d_addr;

	if (addr_12->g.ch != 0 || addr_12->g.lun != 0 || addr_12->g.pg != 0) {
		NVM_DEBUG("FAILED: illegal address for super block");
		errno = EIO;
		return -1;
	}

	addr_20 = nvm_be_addr_format_glue(dev, addr_12);
	addr_20_tmp->l.chunk = addr_20.l.chunk;

	for (npunit_i = 0; npunit_i < npunit; npunit_i++) {
		addr_20_tmp->l.punit = npunit_i;

		for (npugrp_i = 0; npugrp_i < npugrp; npugrp_i++) {
			uint8_t chunk_state;

			addr_20_tmp->l.pugrp = npugrp_i;

			/* Check whether this blk is offline */
			rc = beta_banana_dev_chunk_is_offline(dev, addr_20_tmp);
			if (rc) {
				continue;
			}

			/* Check whether this blk is in vacant or closed state */
			chunk_state = beta_banana_dev_get_chunk_state(dev, addr_20_tmp);
			NVM_DEBUG_DETAIL("Chunk(ch %d, lun %d, chunk %d) state is %d", npugrp_i, npunit_i, addr_20.l.chunk, chunk_state);
			if (chunk_state == NVM_CHUNK_STATE_FREE) {
				NVM_DEBUG("Chunk(ch %d, lun %d, chunk %d) is already in FREE state", npugrp_i, npunit_i, addr_20.l.chunk);
				continue;
			} else if (chunk_state == NVM_CHUNK_STATE_OPEN) {
				fprintf(stderr, "Chunk(ch %d, lun %d, chunk %d) is in OPEN state\n", npugrp_i, npunit_i, addr_20.l.chunk);
				continue;
			}

			d_addr = nvm_addr_gen2dev(dev, addr_20_tmp[0]);

			spdk_ocssd_banana_chunk_reset_cmd(&cmd, d_addr, BANANA_DEV_PHY_RESET);
			rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, NULL, 0,
					NULL, 0, &internal_ret);
			if (ret != NULL) {
				*ret = *(struct nvm_ret *)&internal_ret;
			}

			if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
				NVM_DEBUG("FAILED: nvm_addr_beta_banana_erase_sb");
				return -1;
			}
		}
	}

	return 0;
}

ssize_t nvm_addr_beta_banana_erase_sb(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		  uint16_t flags, struct nvm_ret *ret)
{
	int i;

	for (i = 0; i < naddrs; i++) {
		nvm_addr_beta_banana_erase_one_sb(dev, &addrs_12[i],
				  flags, ret);
	}

	return 0;
}

static ssize_t nvm_addr_beta_banana_parity_init(struct nvm_dev *dev, struct nvm_addr *p_addr_12, struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {};
	struct spdk_nvme_cmd cmd = {0};
	int rc, i, j;
	uint64_t *chk_ppa_lists;
	uint64_t parity_ppa = 0;
	struct nvm_addr p_addr_20[1];
	struct nvm_vblk *vblk;

	p_addr_20[0] = nvm_be_addr_format_glue(dev, p_addr_12);

	vblk = nvm_vblk_alloc_filtered_line(dev, p_addr_20->l.chunk);
	if (vblk->nblks < 2) {
		errno = EIO;
		goto failed;
	}

	chk_ppa_lists = nvm_buf_alloc(dev, sizeof(uint64_t) * vblk->nblks, NULL);
	for (i = 0, j = 0; i < vblk->nblks; j++, i++) {
			uint64_t blk_ppa = nvm_addr_gen2dev(dev,  vblk->blks[i]);

#ifdef DEBUG_DETAIL
			nvm_addr_prn(&vblk->blks[i], 1, dev);
#endif
			/* extract out parity_ppa from vblks due to AB06 limitation */
			if ((vblk->blks[i].l.pugrp == p_addr_20->l.pugrp) && (vblk->blks[i].l.punit == p_addr_20->l.punit)) {
				parity_ppa = blk_ppa;
				NVM_DEBUG_DETAIL("parity ppa (list_idx %d, blk_idx %d) is %lu", j, i, blk_ppa);
				j--;
			} else {
				chk_ppa_lists[j] = blk_ppa;
				NVM_DEBUG_DETAIL("ppa (list_idx %d, blk_idx %d) is %lu", j, i, blk_ppa);
			}
	}

	spdk_ocssd_banana_parity_init_cmd(&cmd, chk_ppa_lists, vblk->nblks - 2, parity_ppa);

	rc = spdk_bdev_req_io_passthru_sync(bt, &cmd, chk_ppa_lists, 4096,
			NULL, 0, &internal_ret);
	if (ret != NULL) {
		*ret = *(struct nvm_ret *)&internal_ret;
	}

	if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
		NVM_DEBUG("FAILED: nvm_addr_beta_banana_parity_init");
		errno = -rc;
		goto failed;
	}

	beta_banana_dev_vblk_set(dev, p_addr_20->l.chunk, vblk);
	return 0;

failed:
	nvm_vblk_free(vblk);
	return -1;
}

ssize_t nvm_addr_beta_banana_parities_init(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs, struct nvm_ret *ret)
{
	int i;
	ssize_t rc = 0;

	for (i = 0; i < naddrs && rc == 0; i++) {
		rc = nvm_addr_beta_banana_parity_init(dev, &addrs_12[i], ret);
	}

	return rc;
}


static void fill_data(const struct nvm_geo *geo, struct nvm_addr addr, void *data, void *meta)
{
	unsigned long *vd_data = data;
	uint64_t *vd_meta = meta;
	memset(vd_data, 0, geo->sector_nbytes);
	vd_data[0] = (int)addr.g.ch;
	vd_data[1] = (int)addr.g.lun;
	vd_data[2] = (int)addr.g.pl;
	vd_data[3] = (int)addr.g.blk;
	vd_data[4] = (int)addr.g.pg;
	vd_data[5] = (int)addr.g.sec;
	vd_data[6] = 1;
	vd_data[7] = 1;
	if (vd_meta) {
		memset(vd_meta, 0, geo->meta_nbytes);
		*vd_meta = (addr.ppa & 0xFFFFFFFFFFFFULL);
	}
}

static int beta_banana_vblk_fill_out(struct nvm_dev *dev, struct nvm_addr *blks, int nblks,
		uint64_t *wps, struct nvm_addr *p_addr_20)
{
	int i, j, rc;
	int nsectors;
	uint8_t *data;
	struct nvm_addr tmp_addr_20;
	int parity_idx = 0;

	data = nvm_buf_alloc(dev, 4096, NULL);
	if (data == NULL) {
		assert(0);
		return -1;
	}

	tmp_addr_20.l.chunk = p_addr_20->l.chunk;


	for (i = 0; i < nblks; i++) {
		if ((blks[i].l.pugrp == p_addr_20->l.pugrp) && (blks[i].l.punit == p_addr_20->l.punit)) {
			parity_idx = i;
			break;
		}
	}

	/* Let each chunk to be 128KB aligned */
	while (wps[parity_idx] < dev->geo.l.nsectr) {
		for (i = 0; i < nblks; i++) {
			if (i == parity_idx) {
				continue;
			}

			tmp_addr_20.l.punit = blks[i].l.punit;
			tmp_addr_20.l.pugrp = blks[i].l.pugrp;

			assert(wps[i] >= wps[parity_idx]);
			nsectors = wps[i] - wps[parity_idx];
			for (j = 0; j + nsectors < BANANA_DEV_OPT_WRT_NSECTOR; j++) {
				tmp_addr_20.l.sectr = wps[i] + j;

//				nvm_addr_prn(&tmp_addr_20, 1, dev);
				fill_data(&dev->geo, tmp_addr_20, data, NULL);

				rc = nvm_addr_beta_banana_rw(dev, &tmp_addr_20, 1, data, NULL, 0, 0, NULL, 0, 1);
				if (rc != 0) {
					return rc;
				}
			}

			wps[i] += j;
		}

		wps[parity_idx] += BANANA_DEV_OPT_WRT_NSECTOR;
		fprintf(stderr, "parity wp is %lu, nsectr is %lu\n", wps[parity_idx], dev->geo.l.nsectr);
	}

	nvm_buf_free(dev, data);
	return 0;
}

ssize_t nvm_addr_beta_banana_parity_out(struct nvm_dev *dev, struct nvm_addr *p_addr_12, struct nvm_ret *ret)
{

	int rc, i;
	struct nvm_addr p_addr_20;
	struct nvm_vblk *vblk;
	uint64_t parity_wp, wps[128];

	p_addr_20 = nvm_be_addr_format_glue(dev, p_addr_12);

	vblk = beta_banana_dev_vblk_get(dev, p_addr_20.l.chunk);

	/* Check whether wp gap between each chunk is less than 128K.
	 * And parity chunk is the least one
	 */
	parity_wp = beta_banana_dev_get_write_pointer(dev, &p_addr_20);
	for (i = 0; i < vblk->nblks; i++) {
		wps[i] = beta_banana_dev_get_write_pointer(dev, &vblk->blks[i]);
		if (wps[i] < parity_wp || parity_wp % BANANA_DEV_OPT_WRT_NSECTOR) {
			fprintf(stderr, "wp (ch %d, lun %d) is %lu, while parity_wp (ch %d, lun %d) is %lu\n",
					vblk->blks[i].l.pugrp, vblk->blks[i].l.punit, wps[i],
					p_addr_20.l.pugrp, p_addr_20.l.punit, parity_wp);

			return -1;
		}
	}

	/* Write out of each chunk by 128K raid strip write */
	rc = beta_banana_vblk_fill_out(dev, vblk->blks, vblk->nblks, wps, &p_addr_20);

	return rc;
}

ssize_t nvm_addr_beta_banana_parities_out(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs, struct nvm_ret *ret)
{
	int i;
	ssize_t rc = 0;

	for (i = 0; i < naddrs && rc == 0; i++) {
		rc = nvm_addr_beta_banana_parity_out(dev, &addrs_12[i], ret);
	}

	return rc;
}

ssize_t nvm_addr_beta_banana_protected_write(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		void *data, void *meta, uint16_t flags,
		int head_idx, int fake_write, int no_meta, struct nvm_ret *ret)
{
	return nvm_addr_beta_banana_rw(dev, addrs_12, naddrs, (void *)data, (void *)meta, flags, 0, ret, 0, 0);
}

ssize_t nvm_addr_beta_banana_first_read(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 void *data, uint16_t flags, struct nvm_ret *ret)
{
	return -1;
}

ssize_t nvm_addr_beta_banana_over_write_page(struct nvm_dev *dev, struct nvm_addr addrs_12[], int naddrs,
		 int head, struct nvm_ret *ret)
{
	return -1;
}

ssize_t nvm_beta_banana_read_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                 unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	return -1;
}

ssize_t nvm_beta_banana_write_pm(struct nvm_dev* dev, void* buf, uint16_t flags,
                 unsigned int length, unsigned int offset, struct nvm_ret* ret)
{
	return -1;
}

static ssize_t nvm_addr_beta_banana_reserved_rw(struct nvm_dev* dev, struct nvm_addr *addr, int nsectors,
		void* data_buf, uint16_t flags, struct nvm_ret* ret, int read)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_nvme_cmd cmd = {0};
	size_t sector_nbytes = dev->geo.sector_nbytes;
	int rc;

#define BANANA_SYS_RW_NSECTOR 8
	if (nsectors != BANANA_SYS_RW_NSECTOR) {
		NVM_DEBUG("FAILED: nvm_addr_beta_banana_sys_rw requires only 8 nsectors");
		return -1;
	}


	if (read) {
		spdk_ocssd_banana_read_sys_cmd(&cmd);
	} else {
		spdk_ocssd_banana_write_sys_cmd(&cmd);
	}

	rc = spdk_bdev_req_admin_passthru_sync(bt, &cmd, data_buf, sector_nbytes * BANANA_SYS_RW_NSECTOR, &internal_ret);
	if (ret != NULL) {
		*ret = *(struct nvm_ret *)&internal_ret;
	}

	if (rc || spdk_bdev_aio_ret_check(&internal_ret)) {
		NVM_DEBUG("FAILED: nvm_addr_beta_banana_sys_rw");
		return -1;
	}


	return 0;
}

ssize_t nvm_beta_banana_read_reserved(struct nvm_dev* dev, struct nvm_addr *addr, int nsectors,
		void* data_buf, uint16_t flags, struct nvm_ret* ret)
{
	return nvm_addr_beta_banana_reserved_rw(dev, addr, nsectors, data_buf, flags, ret, 1);
}

ssize_t nvm_beta_banana_write_reserved(struct nvm_dev* dev, struct nvm_addr *addr, int nsectors,
		void* data_buf, uint16_t flags, struct nvm_ret* ret)
{
	return nvm_addr_beta_banana_reserved_rw(dev, addr, nsectors, data_buf, flags, ret, 0);
}

ssize_t nvm_beta_banana_get_mef_log(struct nvm_dev* dev, struct nvm_log_page *log)
{
	return -1;
}


struct cmd_ctx_naddr_arg {
	struct cmd_ctx* ctx;
	int cpl_count;
};

static ssize_t _nvm_beta_banana_get_async_cmd_event(struct nvm_dev* dev, struct result_ctx* rctx, int is_nonblock)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_ret internal_ret = {0};
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	int nr_min = 0;
	int nr_max, nr;
	struct spdk_bdev_aio_req *reqs[RESULT_MAX_CMD_CTX];
	int completed_nr, j;

	assert(async_ctx);
	assert(rctx->max_count <= RESULT_MAX_CMD_CTX);

	nr_max = rctx->max_count;
	if (rctx->max_count == 0) {
		nr_max = 1;
	}
	/* rctx->max_count indicates how many ctx are expected by caller before calling get_async_cmd_event.
	 * It also indicates how many ctx are completed after calling get_async_cmd_event.
	 * It shoule be 0 if no ctx is completed.
	 */
	rctx->max_count = 0;
	nr = nr_max - rctx->max_count;

	/* This function can be block mode or nonblock mode */
	if (is_nonblock == 0) {
		nr_min = 1;
	}

	while (nr > 0) {
		completed_nr = spdk_bdev_aio_ctx_get_reqs(async_ctx,
				nr_min, nr_max, reqs, NULL);
		if (completed_nr < 0) {
			errno = completed_nr;
			return -1;
		} else if (completed_nr == 0){
			break;
		}

		for (j = 0; j < completed_nr; j++) {
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

		/* Only continue query completed req if completed_nr == nr_max */
		if (completed_nr < nr_max) {
			break;
		}

		nr = nr_max - rctx->max_count;
	}

	return 0;
}


ssize_t nvm_beta_banana_get_async_cmd_event(struct nvm_dev* dev, struct result_ctx* rctx)
{
	int is_nonblock = 0;

	return _nvm_beta_banana_get_async_cmd_event(dev, rctx, is_nonblock);
}

//TODO: some flags examples?
static ssize_t nvm_addr_beta_banana_async_rw(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta, int cmd_ctx_op_code)
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
		errno = ENOMEM;
		return -1;
	}
	naddr_arg->cpl_count = naddrs;
	naddr_arg->ctx = ctx;

	reqs = calloc(naddrs, sizeof(*reqs));
	if (reqs == NULL) {
		errno = ENOMEM;
		goto failed;
	}
	for (i = 0; i < naddrs; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = ENOMEM;
			goto failed;
		}
	}

	for (i = 0; i < naddrs; i++) {
		data_loop = data + sector_nbytes * i;
		if (meta_nbytes != 0 && meta != NULL) {
			meta_loop = meta + meta_nbytes * i;
		}

		d_addr = nvm_be_addr_gen2dev_glue(dev,  &addrs[i]);
		if (cmd_ctx_op_code == OP_READ) {
			spdk_ocssd_banana_read_cmd(&cmd, d_addr, BANANA_DEV_FAKE_LBA);
		} else if (cmd_ctx_op_code == OP_WRITE) {
			spdk_ocssd_banana_write_cmd(&cmd, d_addr, BANANA_DEV_FAKE_LBA);
		} else {
			goto failed;
		}

		spdk_bdev_aio_req_set_io_passthru(reqs[i], &cmd, data_loop, sector_nbytes,
				meta_loop, meta_nbytes);
		spdk_bdev_aio_req_set_private_arg(reqs[i], naddr_arg);
	}

	ctx->opcode = cmd_ctx_op_code;

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

//TODO: some flags examples?
ssize_t nvm_addr_beta_banana_async_read(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                            int no_meta)
{
	return nvm_addr_beta_banana_async_rw(dev, ctx, flags, no_meta, OP_READ);
}


ssize_t nvm_addr_beta_banana_async_protected_write(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                                   int head_idx, int fake_write, int no_meta)
{
	return nvm_addr_beta_banana_async_rw(dev, ctx, flags, no_meta, OP_WRITE);
}


static ssize_t nvm_addr_beta_banana_async_erase_one_sb(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct spdk_nvme_cmd cmd = {0};
	size_t npugrp = dev->geo.l.npugrp;
	size_t npunit = dev->geo.l.npunit;
	struct nvm_addr addr_20_tmp[1] = {0};
	struct nvm_addr addr_20;
	struct nvm_addr *addr_12;
	uint16_t npugrp_i, npunit_i;
	uint64_t d_addr;
	int rc, req_idx;
	uint32_t i;
	struct spdk_bdev_aio_req *reqs[128] = {0};
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;

	if (ctx->naddrs != 1) {
		NVM_DEBUG("FAILED: Unsupport erasing multiple super block");
		errno = EINVAL;
		return -1;
	}

	addr_12 = &ctx->addrs[0];

	if (addr_12->g.ch != 0 || addr_12->g.lun != 0 || addr_12->g.pg != 0) {
		NVM_DEBUG("FAILED: illegal address for super block");
		errno = EINVAL;
		return -1;
	}

	addr_20 = nvm_be_addr_format_glue(dev, addr_12);
	addr_20_tmp->l.chunk = addr_20.l.chunk;

	naddr_arg = malloc(sizeof(*naddr_arg));
	if (naddr_arg == NULL) {
		errno = ENOMEM;
		return -1;
	}
	naddr_arg->ctx = ctx;

	for (i = 0; i < npunit * npugrp; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = ENOMEM;
			goto failed;
		}
	}

	req_idx = 0;
	for (npunit_i = 0; npunit_i < npunit; npunit_i++) {
		addr_20_tmp->l.punit = npunit_i;

		for (npugrp_i = 0; npugrp_i < npugrp; npugrp_i++) {
			uint8_t chunk_state;

			addr_20_tmp->l.pugrp = npugrp_i;
			/* Check whether this blk is offline */
			rc = beta_banana_dev_chunk_is_offline(dev, addr_20_tmp);
			if (rc) {
				continue;
			}

			/* Check whether this blk is in vacant or closed state */
			chunk_state = beta_banana_dev_get_chunk_state(dev, addr_20_tmp);
			NVM_DEBUG_DETAIL("Chunk(ch %d, lun %d, chunk %d) state is %d", npugrp_i, npunit_i, addr_20.l.chunk, chunk_state);
			if (chunk_state == NVM_CHUNK_STATE_FREE) {
				NVM_DEBUG("Chunk(ch %d, lun %d, chunk %d) is already in FREE state", npugrp_i, npunit_i, addr_20.l.chunk);
				continue;
			} else if (chunk_state == NVM_CHUNK_STATE_OPEN) {
				fprintf(stderr, "Chunk(ch %d, lun %d, chunk %d) is in OPEN state\n", npugrp_i, npunit_i, addr_20.l.chunk);
				continue;
			}

			d_addr = nvm_addr_gen2dev(dev, addr_20_tmp[0]);

			spdk_ocssd_banana_chunk_reset_cmd(&cmd, d_addr, BANANA_DEV_PHY_RESET);


			spdk_bdev_aio_req_set_io_passthru(reqs[req_idx], &cmd, NULL, 0, NULL, 0);
			spdk_bdev_aio_req_set_private_arg(reqs[req_idx], naddr_arg);
			req_idx++;
		}
	}

	if (req_idx == 0) {
		errno = EINVAL;
		goto failed;
	}

	ctx->opcode = OP_WRITE;

	naddr_arg->cpl_count = req_idx;
	rc = spdk_bdev_aio_ctx_submit(async_ctx, req_idx, reqs);
	if (rc) {
		goto failed;
	}

	for (i = req_idx; i < npunit * npugrp; i++) {
		free(reqs[i]);
	}

	return 0;

failed:
	if (reqs != NULL) {
		for (i = 0; i < npunit * npugrp; i++) {
			if (reqs[i] != NULL) {
				free(reqs[i]);
			}
		}
	}

	free(naddr_arg);

	return -1;
}

ssize_t nvm_addr_beta_banana_async_erase_sb(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags)
{
	return nvm_addr_beta_banana_async_erase_one_sb(dev, ctx, flags);
}

ssize_t nvm_addr_beta_banana_async_parity_init(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_aio_ctx *async_ctx = state->async_ctx;
	struct spdk_bdev_aio_req *reqs[1] = {0};
	struct cmd_ctx_naddr_arg *naddr_arg = NULL;
	struct spdk_nvme_cmd cmd = {0};
	int rc, i, j, req_idx;
	uint64_t *chk_ppa_lists;
	uint64_t parity_ppa = 0;
	struct nvm_addr p_addr_20[1];
	struct nvm_vblk *vblk;
	struct nvm_addr *p_addr_12;


	if (ctx->naddrs != 1) {
		NVM_DEBUG("FAILED: Raid Operation requires only 1 addr");
		errno = EINVAL;
		return -1;
	}

	naddr_arg = malloc(sizeof(*naddr_arg));
	if (naddr_arg == NULL) {
		errno = ENOMEM;
		return -1;
	}
	naddr_arg->ctx = ctx;

	for (i = 0; i < 1; i++) {
		reqs[i] = malloc(spdk_bdev_aio_req_size());
		if (reqs[i] == NULL) {
			errno = ENOMEM;
			goto failed;
		}
	}

	p_addr_12 = &ctx->addrs[0];
	p_addr_20[0] = nvm_be_addr_format_glue(dev, p_addr_12);

	vblk = nvm_vblk_alloc_filtered_line(dev, p_addr_20->l.chunk);
	beta_banana_dev_vblk_set(dev, p_addr_20->l.chunk, vblk);

	chk_ppa_lists = nvm_buf_alloc(dev, sizeof(uint64_t) * vblk->nblks, NULL);
	for (i = 0, j = 0; i < vblk->nblks; j++, i++) {
			uint64_t blk_ppa = nvm_addr_gen2dev(dev,  vblk->blks[i]);

#ifdef DEBUG_DETAIL
			nvm_addr_prn(&vblk->blks[i], 1, dev);
#endif
			/* extract out parity_ppa from vblks due to AB06 limitation */
			if ((vblk->blks[i].l.pugrp == p_addr_20->l.pugrp) && (vblk->blks[i].l.punit == p_addr_20->l.punit)) {
				parity_ppa = blk_ppa;
				NVM_DEBUG_DETAIL("parity ppa (list_idx %d, blk_idx %d) is %lu", j, i, blk_ppa);
				j--;
			} else {
				chk_ppa_lists[j] = blk_ppa;
				NVM_DEBUG_DETAIL("ppa (list_idx %d, blk_idx %d) is %lu", j, i, blk_ppa);
			}
	}

	spdk_ocssd_banana_parity_init_cmd(&cmd, chk_ppa_lists, vblk->nblks - 2, parity_ppa);

	req_idx = 0;
	spdk_bdev_aio_req_set_io_passthru(reqs[req_idx], &cmd, chk_ppa_lists, 4096, NULL, 0);
	spdk_bdev_aio_req_set_private_arg(reqs[req_idx], naddr_arg);
	req_idx = 1;


	if (req_idx == 0) {
		errno = EINVAL;
		goto failed;
	}

	ctx->opcode = OP_PARITY_INIT;

	naddr_arg->cpl_count = req_idx;
	rc = spdk_bdev_aio_ctx_submit(async_ctx, req_idx, reqs);
	if (rc) {
		goto failed;
	}

	for (i = req_idx; i < 1; i++) {
		free(reqs[i]);
	}

	return 0;

failed:
	if (reqs != NULL) {
		for (i = 0; i < 1; i++) {
			if (reqs[i] != NULL) {
				free(reqs[i]);
			}
		}
	}

	free(naddr_arg);

	return -1;
}

ssize_t nvm_addr_beta_banana_async_parity_out(struct nvm_dev* dev, struct cmd_ctx* ctx)
{
	return -1;
}

ssize_t nvm_beta_banana_async_read_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                unsigned int length, unsigned int offset)
{
	return -1;
}
ssize_t nvm_beta_banana_async_write_pm(struct nvm_dev* dev, struct cmd_ctx* ctx, uint16_t flags,
                unsigned int length, unsigned int offset)
{
	return -1;
}

/* internal admin basic functions for backend */
struct nvm_spec_rprt *nvm_beta_banana_rprt(struct nvm_dev *dev, struct nvm_addr *addr, int opt,
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
	size_t count;

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

	for (size_t i = 0; i < ndescr; i += 0x1000 / DESCR_NBYTES, lpo_off += count * DESCR_NBYTES) {
		count = NVM_MIN(0x1000 / DESCR_NBYTES, ndescr - i);
		spdk_ocssd_banana_chunkinfo_cmd(&cmd, lpo_off, count);

		rc = spdk_bdev_req_admin_passthru_sync(bt, (struct spdk_nvme_cmd *)&cmd, &chunkinfo_buf[i],
				count, &internal_ret);
		if (ret != NULL) {
			*ret = *(struct nvm_ret *)&internal_ret;
		}

		if (rc|| spdk_bdev_aio_ret_check(&internal_ret)) {
			NVM_DEBUG("FAILED: nvm_be_spdk_advanced_rprt_chunk");
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

static uint8_t
beta_banana_dev_get_chunk_state(struct nvm_dev *dev, struct nvm_addr *addr_20)
{
	struct nvm_spec_rprt *rprt;
	struct nvm_addr addr_12[1] = {};
	int chunk_idx;
	uint8_t chunk_state;

	addr_12->g.ch = addr_20->l.pugrp;
	addr_12->g.lun = addr_20->l.punit;
	chunk_idx = addr_20->l.chunk;

	rprt = nvm_beta_banana_rprt(dev, addr_12, 0, NULL);
	assert(rprt != NULL);

	chunk_state = rprt->descr[chunk_idx].cs;
	free(rprt);

	return chunk_state;
}

static uint64_t beta_banana_dev_get_write_pointer(struct nvm_dev *dev, struct nvm_addr *addr_20)
{
	struct nvm_spec_rprt *rprt;
	struct nvm_addr addr_12[1] = {};
	int chunk_idx;
	uint64_t wp;

	addr_12->g.ch = addr_20->l.pugrp;
	addr_12->g.lun = addr_20->l.punit;
	chunk_idx = addr_20->l.chunk;

	rprt = nvm_beta_banana_rprt(dev, addr_12, 0, NULL);

	assert(rprt != NULL);

#ifdef DEBUG_DETAIL
	nvm_spec_rprt_descr_pr(&rprt->descr[chunk_idx]);
#endif
	wp = rprt->descr[chunk_idx].wp;
	free(rprt);

	return wp;
}

int nvm_beta_banana_gfeat(struct nvm_dev *dev, uint8_t id, union nvm_nvme_feat *feat,
	     struct nvm_ret *ret)
{
	return -1;
}

int nvm_beta_banana_sfeat(struct nvm_dev *dev, uint8_t id, const union nvm_nvme_feat *feat,
	     struct nvm_ret *ret)
{
	return -1;
}

struct beta_banana_dev_ctx {
	/* each vblk will be allocated by vblk API, so here use 2D pointer */
	struct nvm_vblk **nvm_vblks;
	/* Size of bbt structure is uncertain due to indeterminated number of blocks */
	struct nvm_spec_bbt **bbts; // index = idx_channel * nluns + idx_lun
};

/* setup vblks for the device */
static int beta_banana_dev_init_vblk(struct nvm_dev *dev, struct beta_banana_dev_ctx *ctx)
{
	const struct nvm_geo *geo = nvm_dev_get_geo(dev);
	int nchunk;
	struct nvm_vblk **nvm_vblks;

	assert(verid == NVM_SPEC_VERID_20);
	nchunk = geo->l.nchunk;

	nvm_vblks = (struct nvm_vblk **)calloc(nchunk, sizeof(struct nvm_vblk *));
	if (nvm_vblks == NULL) {
		assert(0);
		return -1;
	}

	ctx->nvm_vblks = nvm_vblks;

	return 0;
}

/* Release vblks resource */
static int beta_banana_dev_fini_vblk(struct nvm_dev *dev, struct beta_banana_dev_ctx *ctx)
{
	const struct nvm_geo *geo = nvm_dev_get_geo(dev);
	struct nvm_vblk **nvm_vblks;
	int nchunk;
	int i;

	if (ctx == NULL || ctx->nvm_vblks == NULL) {
		return 0;
	}

	nvm_vblks = ctx->nvm_vblks;
	nchunk = geo->l.nchunk;

	for (i = 0; i < nchunk; i++) {
		if (nvm_vblks[i]) {
			nvm_vblk_free(nvm_vblks[i]);
		}
	}

	return 0;
}

static struct nvm_vblk *
nvm_vblk_alloc_filtered_line(struct nvm_dev *dev, int chunk)
{
	const struct nvm_geo *geo = nvm_dev_get_geo(dev);
	struct nvm_vblk *vblk;
	struct nvm_addr addr_20[1] = {};

	vblk = nvm_vblk_alloc(dev, NULL, 0);
	if (!vblk)
		return NULL;	// Propagate errno

	assert(verid == NVM_SPEC_VERID_20);
	for (uint32_t punit = 0; punit < geo->l.npunit; ++punit) {
		for (uint32_t pugrp = 0; pugrp < geo->l.npugrp; ++pugrp) {
			addr_20->l.pugrp = pugrp;
			addr_20->l.punit = punit;
			addr_20->l.chunk = chunk;

			if (beta_banana_dev_chunk_is_offline(dev, addr_20)) {
				continue;
			}

			vblk->blks[vblk->nblks].ppa = 0;
			vblk->blks[vblk->nblks].l.pugrp = pugrp;
			vblk->blks[vblk->nblks].l.punit = punit;
			vblk->blks[vblk->nblks].l.chunk = chunk;
			++(vblk->nblks);
		}
	}

	vblk->nbytes = vblk->nblks * geo->l.nsectr * geo->l.nbytes;

#ifdef DEBUG_DETAIL
	nvm_vblk_pr(vblk);
#endif
	for (int i = 0; i < vblk->nblks; ++i) {
		if (nvm_addr_check(vblk->blks[i], dev)) {
			NVM_DEBUG("FAILED: nvm_addr_check");
			free(vblk);
			errno = EINVAL;
			return NULL;
		}
	}

	return vblk;
}

static void
beta_banana_dev_vblk_set(struct nvm_dev *dev, int chunk_idx, struct nvm_vblk *vblk)
{
	struct beta_banana_dev_ctx *ctx = dev->beta_dev_ctx;

	if (vblk == NULL)
		return;

	if (ctx->nvm_vblks[chunk_idx]) {
		free(ctx->nvm_vblks[chunk_idx]);
		ctx->nvm_vblks[chunk_idx] = NULL;
	}

	ctx->nvm_vblks[chunk_idx] = vblk;
}

static struct nvm_vblk *
beta_banana_dev_vblk_get(struct nvm_dev *dev, int chunk_idx)
{
	struct beta_banana_dev_ctx *ctx = dev->beta_dev_ctx;
	struct nvm_vblk *vblk;

	if (ctx == NULL || ctx->nvm_vblks == NULL) {
		return NULL;
	}

	vblk = ctx->nvm_vblks[chunk_idx];
	if (vblk != NULL) {
		return vblk;
	}

	/* AB06 doesn't have any way to get the info about constructed RAID.
	 * So just reconstruct vblk and try let caller to try whether it is writable.
	 */
	vblk = nvm_vblk_alloc_filtered_line(dev, chunk_idx);
	ctx->nvm_vblks[chunk_idx] = vblk;

	return vblk;
}

static int
beta_banana_dev_bbt_from_rprt(struct nvm_dev *dev, struct nvm_addr *addr_12, struct nvm_spec_bbt *bbt)
{
	struct nvm_ret ret[1] = {};
	struct nvm_spec_rprt *rprt;
	uint32_t i;

	rprt = nvm_beta_banana_rprt(dev, addr_12, 0, ret);
	if (rprt == NULL) {
		return -ENOMEM;
	}

	assert(rprt->ndescr == dev->geo.l.nchunk);

	bbt->tblid[0] = 'B';
	bbt->tblid[1] = 'B';
	bbt->tblid[2] = 'L';
	bbt->tblid[3] = 'T';
	bbt->verid = 0;
	bbt->revid = 0;

	bbt->tblks = dev->geo.l.nchunk;
	bbt->tfact = 0;
	bbt->tgrown = 0;
	bbt->tdresv = 0;
	bbt->thresv = 0;

	/* Map chunk offline stat to bbt device reserved block */
	for (i = 0; i < bbt->tblks; i++) {
		if (rprt->descr[i].cs == NVM_CHUNK_STATE_OFFLINE) {
			bbt->blk[i] = NVM_BBT_BAD;
			bbt->tfact++;
		}
	}

	free(rprt);

	return 0;
}

static int
beta_banana_dev_chunk_is_offline(struct nvm_dev *dev, struct nvm_addr *addr_20)
{
	struct beta_banana_dev_ctx *ctx = dev->beta_dev_ctx;
	struct nvm_spec_bbt *bbt;
	int bbt_idx;

	bbt_idx = addr_20->l.pugrp * dev->geo.l.npunit + addr_20->l.punit;
	bbt = ctx->bbts[bbt_idx];

	if (bbt->blk[addr_20->l.chunk] != NVM_BBT_FREE) {
		return 1; // it is one offline chunk
	}

	return 0; // it is a healthy chunk
}


/* Emulate bbts for the device */
static int beta_banana_dev_init_bbts(struct nvm_dev *dev, struct beta_banana_dev_ctx *ctx)
{
	const int verid = nvm_dev_get_verid(dev);
	const struct nvm_geo *geo = nvm_dev_get_geo(dev);
	int nchunk, nchannel, nlun;
	struct nvm_spec_bbt **bbts;
	int i, j;
	int rc;

	if (verid == NVM_SPEC_VERID_12) {
		assert(0);
		return -1;
	}

	nchunk = geo->l.nchunk;
	nchannel = geo->l.npugrp;
	nlun = geo->l.npunit;

	bbts = (struct nvm_spec_bbt **)calloc(nchannel * nlun, sizeof(struct nvm_spec_bbt *));
	if (bbts == NULL) {
		assert(0);
		return -1;
	}
	ctx->bbts = bbts;

	for (i = 0; i < nchannel; i++) {
		for (j = 0; j < nlun; j++) {
			int bbt_idx = i * nlun + j;
			struct nvm_spec_bbt *bbt;
			struct nvm_addr addr_12[1] = {};

			bbt = calloc(1, sizeof(struct nvm_spec_bbt) + nchunk);
			if (bbt == NULL) {
				assert(0);
				return -ENOMEM;
			}
			ctx->bbts[bbt_idx] = bbt;

			addr_12->g.ch = i;
			addr_12->g.lun = j;

			rc = beta_banana_dev_bbt_from_rprt(dev, addr_12, bbt);
			if (rc) {
				assert(0);
				return -1;
			}
		}
	}

	return 0;
}

/* Release bbts resource */
static int beta_banana_dev_fini_bbts(struct nvm_dev *dev, struct beta_banana_dev_ctx *ctx)
{
	int i, total_luns;

	if (ctx == NULL || ctx->bbts == NULL) {
		return 0;
	}

	total_luns = dev->geo.g.nchannels * dev->geo.g.nluns;
	for (i = 0; i < total_luns; i++) {
		if (ctx->bbts[i]) {
			free(ctx->bbts[i]);
		}
	}

	free(ctx->bbts);

	return 0;
}

static int beta_banana_dev_init(struct nvm_dev *dev)
{
	struct beta_banana_dev_ctx *ctx;
	int rc;

	ctx = calloc(1, sizeof(*ctx));
	if (!ctx) {
		assert(0);
		return -1;
	}

	dev->beta_dev_ctx = ctx;

	rc = beta_banana_dev_init_bbts(dev, ctx);
	if (rc) {
		assert(0);
		return rc;
	}

	rc = beta_banana_dev_init_vblk(dev, ctx);
	if (rc) {
		assert(0);
		return rc;
	}

	return 0;
}

/* Release the resource */
static int beta_banana_dev_fini(struct nvm_dev *dev)
{
	struct beta_banana_dev_ctx *ctx = dev->beta_dev_ctx;

	beta_banana_dev_fini_vblk(dev, ctx);
	beta_banana_dev_fini_bbts(dev, ctx);

	free(dev->beta_dev_ctx);

	return 0;
}

struct nvm_spec_bbt *nvm_beta_banana_gbbt(struct nvm_dev *dev, struct nvm_addr addr_12,
			     struct nvm_ret *ret)
{
	struct nvm_spec_bbt *bbt = NULL;
	struct beta_banana_dev_ctx *ctx;
	int lun_idx;
	int bbt_size;
	int rc;
	uint32_t i;

	lun_idx = addr_12.g.ch * dev->geo.g.nluns + addr_12.g.lun;
	ctx = dev->beta_dev_ctx;
	bbt_size = sizeof(struct nvm_spec_bbt) + dev->geo.g.nblocks;

	assert(dev->geo.g.nplanes == 1);
	bbt = calloc(1, bbt_size);
	if (bbt == NULL) {
		return NULL;
	}

	rc = beta_banana_dev_bbt_from_rprt(dev, &addr_12, bbt);
	if (rc) {
		assert(0);
		free(bbt);
		return NULL;
	}

	/* Update newest offlined chunks to dev maintained bbt */
	for (i = 0; i < bbt->tblks; i++) {
		if (bbt->blk[i] == NVM_BBT_BAD && ctx->bbts[lun_idx]->blk[i] == NVM_BBT_FREE) {
			NVM_DEBUG("FAILED: new chunk %d offlined in channel %d lun %d.", i, addr_12.g.ch, addr_12.g.lun);

			ctx->bbts[lun_idx]->blk[i] = NVM_BBT_BAD;
			ctx->bbts[lun_idx]->tfact++;
		}
	}

	/* Still copy maintained bbt in case there was sbbt operations */
	memcpy(bbt, ctx->bbts[lun_idx], bbt_size);

	return bbt;
}

/* @param flags 0x0 = GOOD, 0x1 = BAD, 0x2 = GROWN_BAD, as well as access mode,
 * 			matching to the value of enum nvm_bbt_state */
int nvm_beta_banana_sbbt(struct nvm_dev *dev, struct nvm_addr *addrs_12, int naddrs, uint16_t flags,
	    struct nvm_ret *ret)
{
	struct nvm_spec_bbt *bbt;
	struct beta_banana_dev_ctx *ctx;
	int lun_idx;
	int i;

	ctx = dev->beta_dev_ctx;

	for (i = 0; i < naddrs; i++) {
		struct nvm_addr addr_12 = addrs_12[i];

		if (addr_12.g.pl != 0) {
			return -1;
		}

		lun_idx = addr_12.g.ch * dev->geo.g.nluns + addr_12.g.lun;
		bbt = ctx->bbts[lun_idx];

		bbt->blk[addr_12.g.blk] = flags;
	}

	return 0;
}

static int beta_banana_dev_recognition(struct nvm_dev *dev)
{
	//TODO: add recognization

	return 1;
}

struct nvm_beta_spec_dev beta_banana_dev = {
		.beta_dev_recognition	= beta_banana_dev_recognition,
		.beta_spec_dev_init = beta_banana_dev_init,
		.beta_spec_dev_fini = beta_banana_dev_fini,

		.addr_inject_read_failed = nvm_addr_beta_banana_inject_read_failed,
		.addr_inject_write_failed = nvm_addr_beta_banana_inject_write_failed,
		.addr_inject_erase_failed = nvm_addr_beta_banana_inject_erase_failed,
		.clean_error_inject = nvm_beta_banana_clean_error_inject,
		.clean_read_error_inject = nvm_beta_banana_clean_read_error_inject,

		.addr_read = nvm_addr_beta_banana_read,
		.addr_write = nvm_addr_beta_banana_write,
		.addr_erase = nvm_addr_beta_banana_erase,
		.addr_erase_sb = nvm_addr_beta_banana_erase_sb,
		.addr_parity_init = nvm_addr_beta_banana_parities_init,
		.addr_parity_out = nvm_addr_beta_banana_parities_out,
		.addr_protected_write = nvm_addr_beta_banana_protected_write,
//		.addr_first_read = nvm_addr_beta_banana_first_read,
//		.addr_over_write_page = nvm_addr_beta_banana_over_write_page,
//		.read_pm = nvm_beta_banana_read_pm,
//		.write_pm = nvm_beta_banana_write_pm,
//		.get_mef_log = nvm_beta_banana_get_mef_log,
		.read_reserved = nvm_beta_banana_read_reserved,
		.write_reserved = nvm_beta_banana_write_reserved,

		.get_async_cmd_event = nvm_beta_banana_get_async_cmd_event,
		.addr_async_read = nvm_addr_beta_banana_async_read,
		.addr_async_protected_write = nvm_addr_beta_banana_async_protected_write,
		.addr_async_erase_sb = nvm_addr_beta_banana_async_erase_sb,
		.addr_async_parity_init = nvm_addr_beta_banana_async_parity_init,
		.addr_async_parity_out = nvm_addr_beta_banana_async_parity_out,
//		.async_read_pm = nvm_beta_banana_async_read_pm,
//		.async_write_pm = nvm_beta_banana_async_write_pm,

		.rprt = nvm_beta_banana_rprt,
		.gfeat = nvm_beta_banana_gfeat,
		.sfeat = nvm_beta_banana_sfeat,
		.gbbt = nvm_beta_banana_gbbt,
		.sbbt = nvm_beta_banana_sbbt,
};


