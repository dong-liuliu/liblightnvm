/*
 * be_spdk_advanced - Backend using SPDK application framework
 *
 * Copyright (C) Xiaodong Liu <xiaodong.liu@intel.com>
 * All rights reserved.
 */
#include <liblightnvm.h>
#include <nvm_be.h>

#ifndef NVM_BE_SPDK_ADVANCED_ENABLED

struct nvm_be nvm_be_spdk_advanced = {
	.id = NVM_BE_SPDK_ADVANCED,
	.name = "NVM_BE_SPDK_ADVANCED",

	.open = nvm_be_nosys_open,
	.close = nvm_be_nosys_close,

	.idfy = nvm_be_nosys_idfy,
	.rprt = nvm_be_nosys_rprt,
	.gfeat = nvm_be_nosys_gfeat,
	.sfeat = nvm_be_nosys_sfeat,
	.sbbt = nvm_be_nosys_sbbt,
	.gbbt = nvm_be_nosys_gbbt,

	.scalar_erase = nvm_be_nosys_scalar_erase,
	.scalar_write = nvm_be_nosys_scalar_write,
	.scalar_read = nvm_be_nosys_scalar_read,

	.vector_erase = nvm_be_nosys_vector_erase,
	.vector_write = nvm_be_nosys_vector_write,
	.vector_read = nvm_be_nosys_vector_read,
	.vector_copy = nvm_be_nosys_vector_copy,

	.async_init = nvm_be_nosys_async_init,
	.async_term = nvm_be_nosys_async_term,
	.async_poke = nvm_be_nosys_async_poke,
	.async_wait = nvm_be_nosys_async_wait,
};
#else
#include <nvm_async.h>
#include <nvm_dev.h>
#include <nvm_cmd.h>
#include <nvm_sgl.h>
#include <nvm_be.h>
#include <nvm_be_spdk_advanced.h>
#include <nvm_beta_cmd.h>

/* redefine WRT */
#define BANANA_DEV_OPT_WRT_NSECTOR	32 // 128KB / 4KB
#define BANANA_DEV_OPT_WRT_NSECTOR_POWER	5 // 128KB / 4KB

/* this function is used to set geo 12 elements by its 20 elements */
static inline void nvm_be_geo_format_to_s20(struct nvm_geo *geo_12)
{
	geo_12->l.npugrp = geo_12->nchannels;
	geo_12->l.npunit = geo_12->nluns;
	geo_12->l.nchunk = geo_12->nblocks;
	geo_12->l.nsectr = geo_12->npages * geo_12->nsectors;
	geo_12->l.nbytes = geo_12->sector_nbytes;
	geo_12->l.nbytes_oob = geo_12->meta_nbytes / geo_12->nsectors;
}

static inline void nvm_be_geo_format_to_s12(struct nvm_geo *geo_20)
{
	geo_20->nchannels = geo_20->l.npugrp;
	geo_20->nluns = geo_20->l.npunit;
	geo_20->nblocks = geo_20->l.nchunk;
	geo_20->nplanes = 1;
	geo_20->nsectors = BANANA_DEV_OPT_WRT_NSECTOR;
	geo_20->npages = geo_20->l.nsectr / geo_20->nsectors;
	geo_20->sector_nbytes = geo_20->l.nbytes;
	geo_20->page_nbytes = geo_20->l.nbytes * geo_20->nsectors;
	geo_20->meta_nbytes = geo_20->l.nbytes_oob * geo_20->nsectors;

}

static void nvm_spec_idty_s20_to_s12(struct nvm_spec_idfy *idfy_s20, struct nvm_spec_idfy *idfy_s12)
{
	struct nvm_spec_idfy_s12 *s12 = &idfy_s12->s12;
	struct nvm_spec_idfy_s20 *s20 = &idfy_s20->s20;

//	struct nvm_spec_ppaf_nand *ppaf;
	struct nvm_spec_idfy_cgrp *cgrp = s12->grp;

	s12->verid = NVM_SPEC_VERID_12;
	s12->vnvmt = 0;
	s12->cgroups = 1;
	s12->cap = 0x1; /* BBT is enabled by simulation */
	s12->dom = 0x0; /* L2P is only in host; ECC is provided by device */

	s12->ppaf.n.sec_len = BANANA_DEV_OPT_WRT_NSECTOR_POWER;
	s12->ppaf.n.pg_len = s20->lbaf.sectr - s12->ppaf.n.sec_len;
	s12->ppaf.n.blk_len = s20->lbaf.chunk;
	s12->ppaf.n.pl_len = 0;
	s12->ppaf.n.lun_len = s20->lbaf.punit;
	s12->ppaf.n.ch_len = s20->lbaf.pugrp;

	s12->ppaf.n.sec_off = 0;
	s12->ppaf.n.pg_off = s12->ppaf.n.sec_len;
	s12->ppaf.n.blk_off = s12->ppaf.n.pg_off + s12->ppaf.n.pg_len;
	s12->ppaf.n.pl_off = s12->ppaf.n.blk_off + s12->ppaf.n.blk_len;
	s12->ppaf.n.lun_off = s12->ppaf.n.pl_off + s12->ppaf.n.pl_len;
	s12->ppaf.n.ch_off = s12->ppaf.n.lun_off + s12->ppaf.n.lun_len;

	cgrp->mtype = 0;
	cgrp->fmtype = 0;

	cgrp->num_ch = s20->lgeo.npugrp;
	cgrp->num_lun = s20->lgeo.npunit;
	cgrp->num_pln = 1;
	cgrp->num_blk = s20->lgeo.nchunk;
	cgrp->num_pg = s20->lgeo.nsectr / BANANA_DEV_OPT_WRT_NSECTOR;

	cgrp->fpg_sz = BANANA_DEV_OPT_WRT_NSECTOR * 4096;
	/* sector size and meta should be decided by ns-data */
	cgrp->csecs = 4096;
	cgrp->sos = 0;

	cgrp->trdt = s20->perf.trdt;
	cgrp->trdm = s20->perf.trdm;
	cgrp->tprt = s20->perf.twrt;
	cgrp->tprm = s20->perf.twrm;
	cgrp->tbet = s20->perf.tcet;
	cgrp->tbem = s20->perf.tcem;

	cgrp->mpos = 0xFFFFFF;	/* Assume all multi-plane operations are supported */
	cgrp->mccap = 0x1;	/* Assume device is SLC */
	cgrp->cpar = 128;	/* Assume 128 */
	cgrp->mts[0] = 0;	/* Not implemented */
}

struct nvm_spec_idfy *nvm_be_spdk_advanced_geometry(struct nvm_dev *dev,
				       struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct spdk_bdev_ret internal_ret = {0};
	struct nvm_spec_idfy *idfy;
	const size_t idfy_len = sizeof(*idfy);
	int rc;

	struct nvm_nvme_cmd cmd = { 0 };

	cmd.opcode = NVM_AOPC_IDFY;
	cmd.nsid = 1;

	idfy = nvm_buf_alloc(dev, idfy_len, NULL);
	if (!idfy) {
		errno = ENOMEM;
		return NULL;
	}
	memset(idfy, 0, sizeof(*idfy));

	rc = spdk_bdev_req_admin_passthru_sync(bt, (struct spdk_nvme_cmd *)&cmd, idfy, idfy_len, &internal_ret);
	if (ret != NULL) {
		*ret = *(struct nvm_ret *)&internal_ret;
	}

	if (rc|| spdk_bdev_aio_ret_check(&internal_ret)) {
		NVM_DEBUG("FAILED: nvm_be_spdk_advanced_geometry");
		nvm_buf_free(dev, idfy);
		return NULL;
	}

	return idfy;
}

struct nvm_spec_idfy *nvm_be_spdk_advanced_geometry_s12(struct nvm_dev *dev,
				       struct nvm_ret *ret)
{
	struct nvm_spec_idfy *idfy, *idfy_s12;

	idfy = nvm_be_spdk_advanced_geometry(dev, ret);

	if (idfy->s.verid == NVM_SPEC_VERID_12) {
		idfy_s12 = idfy;
	} else if (idfy->s.verid == NVM_SPEC_VERID_20) {
		idfy_s12 = nvm_buf_alloc(dev, sizeof(struct nvm_spec_idfy), NULL);
		assert(idfy_s12);
		memset(idfy_s12, 0, sizeof(struct nvm_spec_idfy));

		nvm_spec_idty_s20_to_s12(idfy, idfy_s12);
		nvm_buf_free(dev, idfy);
	} else {
		nvm_buf_free(dev, idfy);
		idfy_s12 = NULL;
	}

	return idfy_s12;
}

static struct nvm_nvme_ns *nvm_be_spdk_advanced_ns_data(struct nvm_dev *dev,
				       struct nvm_ret *ret)
{
	struct nvm_be_spdk_advanced_state *state = dev->be_state;
	struct spdk_bdev_target *bt = state->bt;
	struct nvm_nvme_ns *ns_data = NULL;
	struct spdk_bdev_ret internal_ret = {};
	const size_t ns_data_len = sizeof(*ns_data);
	int rc;

	struct nvm_nvme_cmd cmd = { 0 };

	cmd.opcode = 0x06;	// Identify Opcode in NVME Spec
	cmd.nsid = 1;

	ns_data = nvm_buf_alloc(dev, ns_data_len, NULL);
	if (!ns_data) {
		errno = ENOMEM;
		return NULL;
	}
	memset(ns_data, 0, sizeof(*ns_data));

	rc = spdk_bdev_req_admin_passthru_sync(bt, (struct spdk_nvme_cmd *)&cmd, ns_data, ns_data_len, &internal_ret);
	if (ret != NULL) {
		*ret = *(struct nvm_ret *)&internal_ret;
	}

	if (rc|| spdk_bdev_aio_ret_check(&internal_ret)) {
		NVM_DEBUG("FAILED: nvm_be_spdk_advanced_ns_data");
		nvm_buf_free(dev, ns_data);
		return NULL;
	}

	return ns_data;
}

void nvm_be_spdk_advanced_close(struct nvm_dev *dev)
{
	struct nvm_be_spdk_advanced_state *state = dev ? dev->be_state : NULL;

	dev->beta_dev->beta_spec_dev_fini(dev);

	if (!state) {
		return;
	}

	spdk_bdev_aio_ctx_destroy(state->async_ctx);

	if (state->bt) {
//		spdk_bt_close(state->bt);
	}

//	spdk_env_unset();

	free(state);

	return;
}

struct nvm_dev *nvm_be_spdk_advanced_open(const char *dev_path, int NVM_UNUSED(flags))
{
	struct nvm_be_spdk_advanced_state *state = NULL;
	struct nvm_dev *dev = NULL;
	int rc;
	char *spdk_conf_path = getenv(SPDK_CONF_PATH_ENV_NAME);
	char *spdk_debug_opt = getenv(SPDK_DEBUG_OPT);
	int debug = 0;

	/* Allocate memory for key structure */
	dev = malloc(sizeof(*dev));
	if (!dev) {
		NVM_DEBUG("FAILED: malloc(nvm_dev)");
		return NULL;	// Propagate `errno` from malloc
	}
	memset(dev, 0, sizeof(*dev));

	state = malloc(sizeof(*state));
	if (!state) {
		NVM_DEBUG("FAILED: malloc(spdk_advanced_be_state)");
		free(dev);
		return NULL;
	}
	memset(state, 0, sizeof(*state));
	dev->be_state = state;

	/* spdk backend set and open */
	if (spdk_debug_opt != NULL) {
		debug = strcmp(spdk_debug_opt, "yes") == 0;
	}

	rc = spdk_env_setup(spdk_conf_path, debug);
	if (rc) {
		NVM_DEBUG("failed to setup SPDK env\n");
		goto failed;
	}

	rc = spdk_bt_open((char *)dev_path, &state->bt);
	if (rc) {
		NVM_DEBUG("Failed to open bt (%s)\n", dev_path);
		goto failed;
	}

	/* Prepare async ctx */
	rc = spdk_bdev_aio_ctx_setup(&state->async_ctx, state->bt);
	if (rc) {
		NVM_DEBUG("FAILED: setup, bdev_aio_ctx: %p, errno: %s",
			  (void*)state->async_ctx, strerror(errno));
		goto failed;
	}


	//be is set in nvm_be_populate which is too late
	dev->be = &nvm_be_spdk_advanced;

	/*get nvme ns data */
	dev->nsid = 1;
	struct nvm_nvme_ns *nsdata = nvm_be_spdk_advanced_ns_data(dev, NULL);
	if (nsdata == NULL) {
		NVM_DEBUG("FAILED: spdk_nvme_ns_get_data");
		goto failed;
	}
	dev->ns = *nsdata;
	nvm_buf_free(dev, nsdata);

	/* find correct device type */
	dev->beta_dev = nvm_beta_dev_recognition(dev);
	if (dev->beta_dev == NULL) {
		NVM_DEBUG("failed to recognize beta device\n");
		goto failed;
	}

	/* Update basic OCSSD admin cmd API from beta cmd API */
	nvm_be_spdk_advanced.rprt = dev->beta_dev->rprt;
	nvm_be_spdk_advanced.gfeat = dev->beta_dev->gfeat;
	nvm_be_spdk_advanced.sfeat = dev->beta_dev->sfeat;
	nvm_be_spdk_advanced.gbbt = dev->beta_dev->gbbt;
	nvm_be_spdk_advanced.sbbt = dev->beta_dev->sbbt;

	/* Address */
	rc = nvm_be_populate(dev, &nvm_be_spdk_advanced);
	if (rc) {
		NVM_DEBUG("FAILED: nvm_be_populate, err(%d)", rc);
		goto failed;
	}

	rc = nvm_be_populate_derived(dev);
	if (rc) {
		NVM_DEBUG("FAILED: nvm_be_populate_derived");
		goto failed;
	}

	nvm_be_geo_format_to_s20(&dev->geo);

	rc = dev->beta_dev->beta_spec_dev_init(dev);
	if (rc) {
		NVM_DEBUG("FAILED: beta_spec_dev_init");
		goto failed;
	}


	NVM_DEBUG("Let's go!");

	return dev;

failed:
	nvm_be_spdk_advanced_close(dev);
	free(dev);
	return NULL;
}

struct nvm_be nvm_be_spdk_advanced = {
	.id = NVM_BE_SPDK_ADVANCED,
	.name = "NVM_BE_SPDK_ADVANCED",

	.open = nvm_be_spdk_advanced_open,
	.close = nvm_be_spdk_advanced_close,

	.idfy = nvm_be_spdk_advanced_geometry_s12,

	.rprt = nvm_be_nosys_rprt,
	.gfeat = nvm_be_nosys_gfeat,
	.sfeat = nvm_be_nosys_sfeat,
	.sbbt = nvm_be_nosys_sbbt,
	.gbbt = nvm_be_nosys_gbbt,

	.scalar_erase = nvm_be_nosys_scalar_erase,
	.scalar_write = nvm_be_nosys_scalar_write,
	.scalar_read = nvm_be_nosys_scalar_read,

	.vector_erase = nvm_be_nosys_vector_erase,
	.vector_write = nvm_be_nosys_vector_write,
	.vector_read = nvm_be_nosys_vector_read,
	.vector_copy = nvm_be_nosys_vector_copy,

	.async_init = nvm_be_nosys_async_init,
	.async_term = nvm_be_nosys_async_term,
	.async_poke = nvm_be_nosys_async_poke,
	.async_wait = nvm_be_nosys_async_wait,
};
#endif
