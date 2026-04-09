/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/** @file
 *
 * @addtogroup nrf_wifi_api_common FMAC API common
 * @{
 *
 * @brief Header containing API declarations for the
 * FMAC IF Layer of the Wi-Fi driver.
 */
#ifndef __FMAC_API_COMMON_H__
#define __FMAC_API_COMMON_H__

#include "osal_api.h"
#include "common/hal_api_common.h"
#include "host_rpu_sys_if.h"
#include <patch_info.h>
#include "fmac_cmd_common.h"
#include "fmac_structs_common.h"


#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/**
 * @brief De-initialize the UMAC IF layer.
 * @param fpriv Pointer to the context of the UMAC IF layer.
 *
 * This function de-initialize the UMAC IF layer of the RPU WLAN FullMAC
 *	    driver. It does the following:
 *		- De-initializes the HAL layer.
 *		- Frees the context for the UMAC IF layer.
 */
void nrf_wifi_fmac_deinit(struct nrf_wifi_fmac_priv *fpriv);


/**
 * @brief Removes a RPU instance.
 * @param fmac_dev_ctx Pointer to the context of the RPU instance to be removed.
 *
 * This is a wrapper function which frees the memory for
 * an RPU instance at the UMAC layer.
 */
void nrf_wifi_fmac_dev_rem(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);


/**
 * @brief Validate the firmware header.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param info Pointer to the address where the firmware information needs to be copied.
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_validate_fw_header(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						 struct nrf70_fw_image_info *info);

/**
 * @brief Parse the Firmware(s) to be loaded to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *	which was passed as \p fmac_dev_ctx parameter via the
 *	\p add_dev_callbk_fn() callback function.
 * @param fw_data Pointer to the address where the firmware data is available.
 * @param fw_size Size of the firmware data.
 * @param fw_info Pointer to the address where the firmware information needs to be copied.
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_parse(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   const void *fw_data,
					   unsigned int fw_size,
					   struct nrf_wifi_fmac_fw_info *fw_info);


/**
 * @brief Reset the processors in the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 *
 * This function resets the processors in the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);

/**
 * @brief Boot the Firmware(s) to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 *
 * This function boots the FullMAC firmware(s) to the RPU WLAN device and waits
 * for the boot to complete and validate the boot signature.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_boot(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);

/**
 * @brief Load a chunk of the Firmware to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 * @param rpu_proc RPU processor type.
 * @param fw_chunk Pointer to the address where the firmware chunk information is available.
 *
 * This function loads a chunk of the firmware to the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_chunk_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   enum RPU_PROC_TYPE rpu_proc,
					   struct nrf_wifi_fmac_fw_chunk_info *fw_chunk);

/**
 * @brief Loads the Firmware(s) to the RPU WLAN device.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device,
 *      which was passed as \p fmac_dev_ctx parameter via the
 *      \p add_dev_callbk_fn() callback function.
 * @param fmac_fw Information about the FullMAC firmware(s) to be loaded.
 *
 * This function loads the FullMAC firmware(s) to the RPU WLAN device.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_fw_load(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_fw_info *fmac_fw);

/**
 * @brief Get FW versions from the RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param fw_ver Pointer to the address where the FW versions needs to be copied.
 *
 * This function is used to get Firmware versions from the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_ver_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					  unsigned int *fw_ver);

/**
 * @brief Configure HE LTF and GI parameters.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param he_ltf HE LTF parameter which will be configured in RPU.
 * @param he_gi HE GI parameter which will be configured in RPU.
 * @param enabled enable/disable HE LTF and GI parameter configured.
 *
 * This function is used to send a command to RPU
 *	    to configure HE LTF and GI parameters in RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_conf_ltf_gi(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       unsigned char he_ltf,
					       unsigned char he_gi,
					       unsigned char enabled);


/**
 * @brief Configure SR coexistence parameters in RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param cmd SR coexistence parameters which will be configured in RPU.
 * @param cmd_len Command length.
 *
 * This function is used to send a command to RPU to configure
 *	    SR coexistence parameters in RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_conf_srcoex(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					       void *cmd, unsigned int cmd_len);


/**
 * @brief Set the Multicast filter address.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface whose state needs to be changed.
 * @param mcast_info Multicast information to be set.
 *
 * This function is used to send a command (%NRF_WIFI_UMAC_CMD_MCAST_FILTER) to
 *	    instruct the firmware to set the multicast filter address to an interface
 *	    with index \p if_idx and parameters specified by \p mcast_info.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_sys_fmac_set_mcast_addr(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						  unsigned char if_idx,
						  struct nrf_wifi_umac_mcast_cfg *mcast_info);


/**
 * @brief Fetch MAC address from OTP.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param vif_idx Interface index for which the MAC address is to be fetched.
 * @param mac_addr Pointer to the address where the MAC address needs to be copied.
 *
 * This function is used to fetch MAC address from the OTP.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_otp_mac_addr_get(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
						    unsigned char vif_idx,
						    unsigned char *mac_addr);


/**
 * @brief Set regulatory domain in RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param reg_info Pointer to the address where the regulatory domain information.
 *		   needs to be copied.
 * This function is used to set regulatory domain in the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_set_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info);

/**
 * @brief Get regulatory domain from RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param reg_info Pointer to the address where the regulatory domain information.
 *		   needs to be copied.
 * This function is used to get regulatory domain from the RPU.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_get_reg(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx,
					   struct nrf_wifi_fmac_reg_info *reg_info);

/**
 * @brief Get the current power save information.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 * @param if_idx Index of the interface on which power management is to be set.
 *
 * This function is used to send a command
 *	    to RPU to Enable/Disable WLAN Power management.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_sys_fmac_get_power_save_info(void *fmac_dev_ctx,
						       unsigned char if_idx);

/**
 * @brief Issue a request to reset stats of the RPU.
 * @param fmac_dev_ctx Pointer to the UMAC IF context for a RPU WLAN device.
 *
 * This function is used to send a command to
 * instruct the firmware to reset current RPU statistics.
 *
 * @return Command execution status
 */
enum nrf_wifi_status nrf_wifi_fmac_stats_reset(struct nrf_wifi_fmac_dev_ctx *fmac_dev_ctx);

/**
 * @}
 */
#endif /* __FMAC_API_COMMON_H__ */
