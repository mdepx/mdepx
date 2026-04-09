/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing VIF handling specific definitions for the
 * FMAC IF Layer of the Wi-Fi driver.
 */

#include "system/fmac_vif.h"

#include "host_rpu_umac_if.h"
#include "common/fmac_util.h"


int nrf_wifi_fmac_vif_check_if_limit(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				     int if_type)
{
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx;

	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	switch (if_type) {
	case NRF_WIFI_IFTYPE_STATION:
	case NRF_WIFI_IFTYPE_P2P_CLIENT:
	case NRF_WIFI_STA_TX_INJECTOR:
	case NRF_WIFI_STA_PROMISC:
	case NRF_WIFI_STA_PROMISC_TX_INJECTOR:
		if (sys_dev_ctx->num_sta > MAX_NUM_STAS) {
			nrf_wifi_osal_log_err("%s: Maximum STA Interface type exceeded",
					      __func__);
			return -1;
		}
		break;
	case NRF_WIFI_IFTYPE_AP:
	case NRF_WIFI_IFTYPE_P2P_GO:
		if (sys_dev_ctx->num_ap > MAX_NUM_APS) {
			nrf_wifi_osal_log_err("%s: Maximum AP Interface type exceeded",
					      __func__);
			return -1;
		}
		break;
	default:
		nrf_wifi_osal_log_err("%s: Interface type %d not supported",
				      __func__, if_type);
		return -1;
	}

	return 0;
}


void nrf_wifi_fmac_vif_incr_if_type(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				    int if_type)
{
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx;

	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	switch (if_type) {
	case NRF_WIFI_IFTYPE_STATION:
	case NRF_WIFI_IFTYPE_P2P_CLIENT:
	case NRF_WIFI_STA_TX_INJECTOR:
	case NRF_WIFI_STA_PROMISC:
	case NRF_WIFI_STA_PROMISC_TX_INJECTOR:
		sys_dev_ctx->num_sta++;
		break;
	case NRF_WIFI_IFTYPE_AP:
	case NRF_WIFI_IFTYPE_P2P_GO:
		sys_dev_ctx->num_ap++;
		break;
	default:
		nrf_wifi_osal_log_err("%s: Unsupported VIF type %d",
				      __func__, if_type);
	}
}


void nrf_wifi_fmac_vif_decr_if_type(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				    int if_type)
{
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx;

	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	switch (if_type) {
	case NRF_WIFI_IFTYPE_STATION:
	case NRF_WIFI_IFTYPE_P2P_CLIENT:
	case NRF_WIFI_STA_TX_INJECTOR:
	case NRF_WIFI_STA_PROMISC:
	case NRF_WIFI_STA_PROMISC_TX_INJECTOR:
		sys_dev_ctx->num_sta--;
		break;
	case NRF_WIFI_IFTYPE_AP:
	case NRF_WIFI_IFTYPE_P2P_GO:
		sys_dev_ctx->num_ap--;
		break;
	default:
		nrf_wifi_osal_log_err("%s: Unsupported VIF type %d",
				      __func__, if_type);
	}
}


void nrf_wifi_fmac_vif_clear_ctx(void *dev_ctx,
				 unsigned char if_idx)
{
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = NULL;
	struct nrf_wifi_fmac_vif_ctx *vif_ctx = NULL;
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx;

	fmac_dev_ctx = dev_ctx;
	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	vif_ctx = sys_dev_ctx->vif_ctx[if_idx];

	nrf_wifi_osal_mem_free(vif_ctx);
	sys_dev_ctx->vif_ctx[if_idx] = NULL;
}


void nrf_wifi_fmac_vif_update_if_type(void *dev_ctx,
				      unsigned char if_idx,
				      int if_type)
{
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = NULL;
	struct nrf_wifi_fmac_vif_ctx *vif_ctx = NULL;
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx = NULL;

	fmac_dev_ctx = dev_ctx;
	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	vif_ctx = sys_dev_ctx->vif_ctx[if_idx];

	nrf_wifi_fmac_vif_decr_if_type(fmac_dev_ctx,
				       vif_ctx->if_type);

	nrf_wifi_fmac_vif_incr_if_type(fmac_dev_ctx,
				       if_type);

	vif_ctx->if_type = if_type;
}

unsigned int nrf_wifi_fmac_get_num_vifs(void *dev_ctx)
{
	struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx = dev_ctx;
	struct nrf_wifi_sys_fmac_dev_ctx *sys_dev_ctx = NULL;

	sys_dev_ctx = wifi_dev_priv(fmac_dev_ctx);

	if (!fmac_dev_ctx) {
		/* Don't return error here, as FMAC might be initialized based
		 * the number of VIFs.
		 */
		return 0;
	}

	return sys_dev_ctx->num_sta + sys_dev_ctx->num_ap;
}
