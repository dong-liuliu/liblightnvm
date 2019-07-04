/*
 * nvm_be_spdk_advanced - internal header for SPDK
 *
 * Copyright (C) Simon A. F. Lund <slund@cnexlabs.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __INTERNAL_NVM_BE_SPDK_ADVANCED_H
#define __INTERNAL_NVM_BE_SPDK_ADVANCED_H
#include <spdk/stdinc.h>
#include <spdk/env.h>
#include <spdk/bdev_target.h>

/* Specify spdk config file path by env variable SPDK_CONF_PATH */
#define SPDK_CONF_PATH_ENV_NAME	"SPDK_CONF_PATH"
/* Enable spdk debug information by set env variable SPDK_DEBUG_OPT to be "yes" */
#define SPDK_DEBUG_OPT			"SPDK_DEBUG_OPT"
#define NVM_APP_ADDR_12

/**
 * Internal representation of NVM_BE_SPDK_ADVANCED state
 */
struct nvm_be_spdk_advanced_state {
	struct spdk_bdev_target *bt;
	struct spdk_bdev_aio_ctx *async_ctx;
};

struct nvm_be_spdk_async_state {
	struct spdk_bdev_aio_ctx *aio_ctx;
	struct spdk_bdev_aio_req **reqs;
};

void nvm_be_spdk_advanced_close(struct nvm_dev *dev);

struct nvm_dev *nvm_be_spdk_advanced_open(const char *dev_path, int flags);

struct nvm_addr nvm_be_addr_format_glue(struct nvm_dev *dev, struct nvm_addr *addr_12);
uint64_t nvm_be_addr_gen2dev_glue(struct nvm_dev *dev, struct nvm_addr *addr_12);

#endif /* __INTERNAL_NVM_BE_SPDK */
