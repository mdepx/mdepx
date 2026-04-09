/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing API definitions for the
 * FMAC IF Layer of the Wi-Fi driver.
 */

#include "host_rpu_umac_if.h"
#include "common/fmac_api_common.h"
#include "common/fmac_util.h"
#include "common/fmac_cmd_common.h"
#include "util.h"


void nrf_wifi_fmac_deinit(struct nrf_wifi_fmac_priv *fpriv)
{
	nrf_wifi_hal_deinit(fpriv->hpriv);

	nrf_wifi_osal_mem_free(fpriv);
}


void nrf_wifi_fmac_dev_rem(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	nrf_wifi_hal_dev_rem(fmac_dev_ctx->hal_dev_ctx);

	nrf_wifi_osal_mem_free(fmac_dev_ctx);
}

struct nrf_wifi_proc {
	const enum RPU_PROC_TYPE type;
	const char *name;
	bool is_patch_present;
};

struct nrf_wifi_proc wifi_proc[] = {
	{RPU_PROC_TYPE_MCU_LMAC, "LMAC", true},
	{RPU_PROC_TYPE_MCU_UMAC, "UMAC", true},
};

static int nrf_wifi_patch_version_compat(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
				const unsigned int version)
{
	unsigned int family, major, minor, patch;

	family = (version >> 24) & 0xff;
	major = (version >> 16) & 0xff;
	minor = (version >> 8) & 0xff;
	patch = (version >> 0) & 0xff;

	if (family != RPU_FAMILY) {
		nrf_wifi_osal_log_err("Incompatible RPU version: %d, expected: %d",
				      family, RPU_FAMILY);
		return -1;
	}

	if (major != RPU_MAJOR_VERSION) {
		nrf_wifi_osal_log_err("Incompatible RPU major version: %d, expected: %d",
				      major, RPU_MAJOR_VERSION);
		return -1;
	}

	/* TODO: Allow minor version to be different */
	if (minor != RPU_MINOR_VERSION) {
		nrf_wifi_osal_log_err("Incompatible RPU minor version: %d, expected: %d",
				      minor, RPU_MINOR_VERSION);
		return -1;
	}

	/* TODO: Allow patch version to be different */
	if (patch != RPU_PATCH_VERSION) {
		nrf_wifi_osal_log_err("Incompatible RPU patch version: %d, expected: %d",
				      patch, RPU_PATCH_VERSION);
		return -1;
	}

	return 0;
}

static int nrf_wifi_patch_feature_flags_compat(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       const unsigned int feature_flags)
{
	switch (fmac_dev_ctx->op_mode) {
	case NRF_WIFI_OP_MODE_RT:
		if (!(feature_flags & NRF70_FEAT_RADIO_TEST)) {
			nrf_wifi_osal_log_err("Radio test feature flag not set");
			return -1;
		}
		break;

	case NRF_WIFI_OP_MODE_SYS:
#ifdef NRF70_SCAN_ONLY
		if (!(feature_flags & NRF70_FEAT_SCAN_ONLY)) {
			nrf_wifi_osal_log_err("Scan only feature flag not set");
			return -1;
		}
#elif defined(NRF70_SYSTEM_MODE)
		if (!(feature_flags & NRF70_FEAT_SYSTEM_MODE) &&
		    !(feature_flags & NRF70_FEAT_SYSTEM_WITH_RAW_MODES)) {
			nrf_wifi_osal_log_err("System mode or system with raw mode feature flag not set");
			return -1;
		}
#else
		nrf_wifi_osal_log_err("Invalid feature flags: 0x%x or build configuration",
				      feature_flags);
		return -1;
#endif
		break;

	case NRF_WIFI_OP_MODE_OFF_RAW_TX:
		if (!(feature_flags & NRF70_FEAT_OFFLOADED_RAW_TX)) {
			nrf_wifi_osal_log_err("Offloaded raw tx feature flag not set");
			return -1;
		}
		break;

	default:
		nrf_wifi_osal_log_err("Invalid op_mode: %d", fmac_dev_ctx->op_mode);
		return -1;
	}

	return 0;
}

enum nrf_wifi_status nrf_wifi_validate_fw_header(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						 struct nrf70_fw_image_info *info)
{

	nrf_wifi_osal_log_dbg("Signature: 0x%x", info->signature);
	nrf_wifi_osal_log_dbg("num_images: %d", info->num_images);
	nrf_wifi_osal_log_dbg("version: 0x%x", info->version);
	nrf_wifi_osal_log_dbg("feature_flags: %d", info->feature_flags);

	if (info->signature != NRF_WIFI_PATCH_SIGNATURE) {
		nrf_wifi_osal_log_err("Invalid patch signature: 0x%x, expected: 0x%x",
				      info->signature, NRF_WIFI_PATCH_SIGNATURE);
		return NRF_WIFI_STATUS_FAIL;
	}

	if (info->num_images != NRF_WIFI_PATCH_NUM_IMAGES) {
		nrf_wifi_osal_log_err("Invalid number of images, expected %d, got %d",
				      NRF_WIFI_PATCH_NUM_IMAGES, info->num_images);
		return NRF_WIFI_STATUS_FAIL;
	}

	if (nrf_wifi_patch_version_compat(fmac_dev_ctx, info->version) != 0) {
		nrf_wifi_osal_log_err("Incompatible patch");
		return NRF_WIFI_STATUS_FAIL;
	}

	if (nrf_wifi_patch_feature_flags_compat(fmac_dev_ctx, info->feature_flags) != 0) {
		nrf_wifi_osal_log_err("Incompatible feature flags");
		return NRF_WIFI_STATUS_FAIL;
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_parse(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   const void *fw_data,
					   unsigned int fw_size,
					   struct nrf_wifi_fmac_fw_info *fw_info)
{
	struct nrf70_fw_image_info *info = (struct nrf70_fw_image_info *)fw_data;
	unsigned int offset;
	unsigned int image_id;

	if (!fw_data || !fw_size || !fw_info) {
		nrf_wifi_osal_log_err("Invalid parameters");
		return NRF_WIFI_STATUS_FAIL;
	}

	if (fw_size < sizeof(struct nrf70_fw_image_info)) {
		nrf_wifi_osal_log_err("Invalid fw_size: %d, minimum size: %d",
			fw_size, sizeof(struct nrf70_fw_image_info));
		return NRF_WIFI_STATUS_FAIL;
	}


	if (nrf_wifi_validate_fw_header(fmac_dev_ctx, info) != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("Invalid fw header");
		return NRF_WIFI_STATUS_FAIL;
	}

	offset = sizeof(struct nrf70_fw_image_info);

	nrf_wifi_osal_log_dbg("====");
	for (image_id = 0; image_id < info->num_images; image_id++) {
		struct nrf70_fw_image *image =
			(struct nrf70_fw_image *)((char *)fw_data + offset);
		const void *data = (char *)fw_data + offset + sizeof(struct nrf70_fw_image);

		if (offset + sizeof(struct nrf70_fw_image) + image->len > fw_size) {
			nrf_wifi_osal_log_err("Invalid fw_size: %d for image[%d] len: %d",
					      fw_size, image_id, image->len);
			return NRF_WIFI_STATUS_FAIL;
		}

		nrf_wifi_osal_log_dbg("image[%d] type: %d", image_id, image->type);
		nrf_wifi_osal_log_dbg("image[%d] len: %d", image_id, image->len);
		nrf_wifi_osal_log_dbg("====");

		switch (image_id) {
		case NRF70_IMAGE_LMAC_PRI:
			fw_info->lmac_patch_pri.data = data;
			fw_info->lmac_patch_pri.size = image->len;
			break;
		case NRF70_IMAGE_LMAC_SEC:
			fw_info->lmac_patch_sec.data = data;
			fw_info->lmac_patch_sec.size = image->len;
			break;
		case NRF70_IMAGE_UMAC_PRI:
			fw_info->umac_patch_pri.data = data;
			fw_info->umac_patch_pri.size = image->len;
			break;
		case NRF70_IMAGE_UMAC_SEC:
			fw_info->umac_patch_sec.data = data;
			fw_info->umac_patch_sec.size = image->len;
			break;
		default:
			nrf_wifi_osal_log_err("Invalid image id: %d", image_id);
			break;
		}

		offset += sizeof(struct nrf70_fw_image) + image->len;
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(wifi_proc); i++) {
		status = nrf_wifi_hal_proc_reset(fmac_dev_ctx->hal_dev_ctx,
						 wifi_proc[i].type);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: %s processor reset failed",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}
	}

	return NRF_WIFI_STATUS_SUCCESS;
}

enum nrf_wifi_status nrf_wifi_fmac_fw_boot(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	int i = 0;

	for (i = 0; i < ARRAY_SIZE(wifi_proc); i++) {
		status = nrf_wifi_hal_fw_patch_boot(fmac_dev_ctx->hal_dev_ctx,
						    wifi_proc[i].type,
						    wifi_proc[i].is_patch_present);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: %s processor ROM boot failed",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}

		status = nrf_wifi_hal_fw_chk_boot(fmac_dev_ctx->hal_dev_ctx,
						  wifi_proc[i].type);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: %s processor ROM boot check failed",
					      __func__, wifi_proc[i].name);
			return NRF_WIFI_STATUS_FAIL;
		}
	}

	return NRF_WIFI_STATUS_SUCCESS;
}


enum nrf_wifi_status nrf_wifi_fmac_fw_chunk_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   enum RPU_PROC_TYPE rpu_proc,
					   struct nrf_wifi_fmac_fw_chunk_info *fw_chunk)
{
	return hal_fw_patch_chunk_load(fmac_dev_ctx->hal_dev_ctx,
				       rpu_proc,
				       fw_chunk->dest_addr,
				       fw_chunk->data,
				       fw_chunk->size);
}


enum nrf_wifi_status nrf_wifi_fmac_fw_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_fw_info *fmac_fw)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = nrf_wifi_fmac_fw_reset(fmac_dev_ctx);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: FW reset failed",
				      __func__);
		goto out;
	}

	/* Load the UMAC patches if available */
	if (fmac_fw->umac_patch_pri.data && fmac_fw->umac_patch_pri.size &&
	    fmac_fw->umac_patch_sec.data && fmac_fw->umac_patch_sec.size) {
		status = nrf_wifi_hal_fw_patch_load(fmac_dev_ctx->hal_dev_ctx,
						    RPU_PROC_TYPE_MCU_UMAC,
						    fmac_fw->umac_patch_pri.data,
						    fmac_fw->umac_patch_pri.size,
						    fmac_fw->umac_patch_sec.data,
						    fmac_fw->umac_patch_sec.size);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: UMAC patch load failed",
					      __func__);
			goto out;
		} else {
			nrf_wifi_osal_log_dbg("%s: UMAC patches loaded",
					      __func__);
		}
	} else {
		wifi_proc[1].is_patch_present = false;
	}

	/* Load the LMAC patches if available */
	if (fmac_fw->lmac_patch_pri.data && fmac_fw->lmac_patch_pri.size &&
	    fmac_fw->lmac_patch_sec.data && fmac_fw->lmac_patch_sec.size) {
		status = nrf_wifi_hal_fw_patch_load(fmac_dev_ctx->hal_dev_ctx,
						    RPU_PROC_TYPE_MCU_LMAC,
						    fmac_fw->lmac_patch_pri.data,
						    fmac_fw->lmac_patch_pri.size,
						    fmac_fw->lmac_patch_sec.data,
						    fmac_fw->lmac_patch_sec.size);

		if (status != NRF_WIFI_STATUS_SUCCESS) {
			nrf_wifi_osal_log_err("%s: LMAC patch load failed",
					      __func__);
			goto out;
		} else {
			nrf_wifi_osal_log_dbg("%s: LMAC patches loaded",
					      __func__);
		}
	} else {
		wifi_proc[0].is_patch_present = false;
	}

	status = nrf_wifi_fmac_fw_boot(fmac_dev_ctx);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: FW boot failed",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->fw_boot_done = true;

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_ver_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					  unsigned int *fw_ver)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	status = hal_rpu_mem_read(fmac_dev_ctx->hal_dev_ctx,
				  fw_ver,
				  RPU_MEM_UMAC_VER,
				  sizeof(*fw_ver));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Unable to read UMAC ver",
				      __func__);
		goto out;
	}

out:
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_otp_mac_addr_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						    unsigned char vif_idx,
						    unsigned char *mac_addr)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_fmac_otp_info otp_info;
	unsigned char *otp_mac_addr = NULL;
	unsigned int otp_mac_addr_flag_mask = 0;

	if (!fmac_dev_ctx || !mac_addr || (vif_idx >= MAX_NUM_VIFS)) {
		nrf_wifi_osal_log_err("%s: Invalid parameters",
				      __func__);
		goto out;
	}

	nrf_wifi_osal_mem_set(&otp_info,
			      0xFF,
			      sizeof(otp_info));

	status = nrf_wifi_hal_otp_info_get(fmac_dev_ctx->hal_dev_ctx,
					   &otp_info.info,
					   &otp_info.flags);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Fetching of RPU OTP information failed",
				      __func__);
		goto out;
	}

	if (vif_idx == 0) {
		otp_mac_addr = (unsigned char *)otp_info.info.mac_address0;
		otp_mac_addr_flag_mask = (~MAC0_ADDR_FLAG_MASK);

	} else if (vif_idx == 1) {
		otp_mac_addr = (unsigned char *)otp_info.info.mac_address1;
		otp_mac_addr_flag_mask = (~MAC1_ADDR_FLAG_MASK);
	}

	/* Check if a valid MAC address has been programmed in the OTP */

	if (otp_info.flags & otp_mac_addr_flag_mask) {
		nrf_wifi_osal_log_info("%s: MAC addr not programmed in OTP",
				       __func__);

	} else {
		nrf_wifi_osal_mem_cpy(mac_addr,
				      otp_mac_addr,
				      NRF_WIFI_ETH_ADDR_LEN);

		if (!nrf_wifi_utils_is_mac_addr_valid((const char *)mac_addr)) {
			nrf_wifi_osal_log_info("%s:  Invalid OTP MA: %02X%02X%02X%02X%02X%02X",
					       __func__,
					       (*(mac_addr + 0)),
					       (*(mac_addr + 1)),
					       (*(mac_addr + 2)),
					       (*(mac_addr + 3)),
					       (*(mac_addr + 4)),
					       (*(mac_addr + 5)));

		}
	}
out:
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_get_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_umac_cmd_get_reg *get_reg_cmd = NULL;
	unsigned int count = 0;

	if (!fmac_dev_ctx || !reg_info) {
		nrf_wifi_osal_log_err("%s: Invalid parameters",
				      __func__);
		goto err;
	}

	nrf_wifi_osal_log_dbg("%s: Get regulatory information", __func__);

	get_reg_cmd = nrf_wifi_osal_mem_zalloc(sizeof(*get_reg_cmd));

	if (!get_reg_cmd) {
		nrf_wifi_osal_log_err("%s: Unable to allocate memory",
				      __func__);
		goto err;
	}

	get_reg_cmd->umac_hdr.cmd_evnt = NRF_WIFI_UMAC_CMD_GET_REG;
	get_reg_cmd->umac_hdr.ids.valid_fields = 0;

	fmac_dev_ctx->alpha2_valid = false;
	fmac_dev_ctx->reg_chan_info = reg_info->reg_chan_info;

	status = umac_cmd_cfg(fmac_dev_ctx,
			      get_reg_cmd,
			      sizeof(*get_reg_cmd));

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Failed to get regulatory information",	__func__);
		goto err;
	}

	do {
		nrf_wifi_osal_sleep_ms(100);
	} while (count++ < 100 && !fmac_dev_ctx->alpha2_valid);

	if (!fmac_dev_ctx->alpha2_valid) {
		nrf_wifi_osal_log_err("%s: Failed to get regulatory information",
				      __func__);
		goto err;
	}

	nrf_wifi_osal_mem_cpy(reg_info->alpha2,
			      fmac_dev_ctx->alpha2,
			      sizeof(reg_info->alpha2));

	reg_info->reg_chan_count = fmac_dev_ctx->reg_chan_count;

	status = NRF_WIFI_STATUS_SUCCESS;
err:
	if (get_reg_cmd) {
		nrf_wifi_osal_mem_free(get_reg_cmd);
	}
	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_stats_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned char count = 0;

	status = umac_cmd_prog_stats_reset(fmac_dev_ctx);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		goto out;
	}

	do {
		nrf_wifi_osal_sleep_ms(1);
	} while ((fmac_dev_ctx->stats_req == true) &&
		 (count++ < NRF_WIFI_FMAC_STATS_RECV_TIMEOUT));

	if (count == NRF_WIFI_FMAC_STATS_RECV_TIMEOUT) {
		nrf_wifi_osal_log_err("%s: Timed out",
				      __func__);
		goto out;
	}

	status = NRF_WIFI_STATUS_SUCCESS;

out:
	return status;
}


enum nrf_wifi_status nrf_wifi_fmac_conf_srcoex(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       void *cmd, unsigned int cmd_len)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	status = umac_cmd_srcoex(fmac_dev_ctx, cmd, cmd_len);

	return status;
}

enum nrf_wifi_status nrf_wifi_fmac_set_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_cmd_req_set_reg *set_reg_cmd = NULL;
	unsigned int count = 0, max_count = NRF_WIFI_FMAC_REG_SET_TIMEOUT_MS / 20;
	enum nrf_wifi_reg_initiator exp_initiator = NRF_WIFI_REGDOM_SET_BY_USER;
	enum nrf_wifi_reg_type exp_reg_type = NRF_WIFI_REGDOM_TYPE_COUNTRY;
	char exp_alpha2[NRF_WIFI_COUNTRY_CODE_LEN] = {0};
	struct nrf_wifi_fmac_reg_info cur_reg_info = {0};
	struct nrf_wifi_event_regulatory_change *reg_change = NULL;

	if (!fmac_dev_ctx || !reg_info) {
		nrf_wifi_osal_log_err("%s: Invalid parameters",
				      __func__);
		goto out;
	}

	nrf_wifi_osal_log_dbg("%s: Setting regulatory information: %c%c", __func__,
	                      reg_info->alpha2[0],
	                      reg_info->alpha2[1]);

	/* No change event from UMAC for same regd */
	status = nrf_wifi_fmac_get_reg(fmac_dev_ctx, &cur_reg_info);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Failed to get current regulatory information",
				      __func__);
		goto out;
	}

	if (nrf_wifi_osal_mem_cmp(cur_reg_info.alpha2,
				  reg_info->alpha2,
				  NRF_WIFI_COUNTRY_CODE_LEN) == 0) {
		nrf_wifi_osal_log_dbg("%s: Regulatory domain already set to %c%c",
				      __func__,
				      reg_info->alpha2[0],
				      reg_info->alpha2[1]);
		status = NRF_WIFI_STATUS_SUCCESS;
		goto out;
	}

	set_reg_cmd = nrf_wifi_osal_mem_zalloc(sizeof(*set_reg_cmd));

	if (!set_reg_cmd) {
		nrf_wifi_osal_log_err("%s: Unable to allocate memory",
				      __func__);
		goto out;
	}

	set_reg_cmd->umac_hdr.cmd_evnt = NRF_WIFI_UMAC_CMD_REQ_SET_REG;
	set_reg_cmd->umac_hdr.ids.valid_fields = 0;

	nrf_wifi_osal_mem_cpy(set_reg_cmd->nrf_wifi_alpha2,
			      reg_info->alpha2,
			      NRF_WIFI_COUNTRY_CODE_LEN);

	exp_alpha2[0] = reg_info->alpha2[0];
	exp_alpha2[1] = reg_info->alpha2[1];

	if (reg_info->alpha2[0] == '0' && reg_info->alpha2[1] == '0') {
		exp_reg_type = NRF_WIFI_REGDOM_TYPE_WORLD;
	}

	set_reg_cmd->valid_fields = NRF_WIFI_CMD_REQ_SET_REG_ALPHA2_VALID;

	/* New feature in rev B patch */
	if (reg_info->force) {
		set_reg_cmd->valid_fields |= NRF_WIFI_CMD_REQ_SET_REG_USER_REG_FORCE;
	}

	fmac_dev_ctx->reg_set_status = false;
	fmac_dev_ctx->waiting_for_reg_event = true;

	status = umac_cmd_cfg(fmac_dev_ctx,
			      set_reg_cmd,
			      sizeof(*set_reg_cmd));
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Failed to set regulatory information",
				      __func__);
		goto out;
	}

	fmac_dev_ctx->reg_set_status = false;
	nrf_wifi_osal_log_dbg("%s: Waiting for regulatory domain change event", __func__);
	while (!fmac_dev_ctx->reg_set_status && count++ <= max_count) {
		nrf_wifi_osal_sleep_ms(100);
	}

	if (!fmac_dev_ctx->reg_set_status) {
		nrf_wifi_osal_log_err("%s: Failed to set regulatory information",
				      __func__);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	fmac_dev_ctx->waiting_for_reg_event = false;
	reg_change = fmac_dev_ctx->reg_change;

	if (reg_change->intr != exp_initiator) {
		nrf_wifi_osal_log_err("%s: Non-user initiated reg domain change: exp: %d, got: %d",
				      __func__,
				      exp_initiator,
				      reg_change->intr);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	if (reg_change->regulatory_type != exp_reg_type) {
		nrf_wifi_osal_log_err("%s: Unexpected reg domain change: exp: %d, got: %d",
				      __func__,
				      exp_reg_type,
				      reg_change->regulatory_type);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

	if ((reg_change->regulatory_type == NRF_WIFI_REGDOM_TYPE_COUNTRY) &&
		 nrf_wifi_osal_mem_cmp(reg_change->nrf_wifi_alpha2,
				       exp_alpha2,
				       NRF_WIFI_COUNTRY_CODE_LEN) != 0) {
		nrf_wifi_osal_log_err("%s: Unexpected alpha2 reg domain change: "
				      "exp: %c%c, got: %c%c",
				      __func__,
				      exp_alpha2[0],
				      exp_alpha2[1],
				      reg_change->nrf_wifi_alpha2[0],
				      reg_change->nrf_wifi_alpha2[1]);
		status = NRF_WIFI_STATUS_FAIL;
		goto out;
	}

out:
	if (set_reg_cmd) {
		nrf_wifi_osal_mem_free(set_reg_cmd);
	}

	if (reg_change) {
		nrf_wifi_osal_mem_free(reg_change);
		fmac_dev_ctx->reg_change = NULL;
	}

	return status;
}
