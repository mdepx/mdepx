/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/module.h>
#include <linux/kernel.h>

#ifdef NRF70_RADIO_TEST
#include "radio_test/fmac_api.h"
#endif
#ifdef NRF70_SYSTEM_MODE
#include "system/fmac_api.h"
#endif
#include "common/fmac_util.h"
#include "common/pal.h"

/* Add EXPORT_SYMBOL_GPL for each function */
EXPORT_SYMBOL_GPL(nrf_wifi_osal_init);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_deinit);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_unmap);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_dev_deinit);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_add_vif);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_assoc);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_mgmt_tx);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_fw_load);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_chg_sta);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_dma_unmap);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_get_tx_power);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_set_qos_map);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_cpy_to);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_chg_vif);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_rem);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_mem_free);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_set_wiphy_params);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_add);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_read_reg32);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_init);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_vif_clear_ctx);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_cpy_from);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_deinit);
EXPORT_SYMBOL_GPL(umac_cmd_cfg);
EXPORT_SYMBOL_GPL(wifi_dev_priv);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_chg_vif_state);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_mem_zalloc);
EXPORT_SYMBOL_GPL(pal_ops_get_fw_loc);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_mac_addr);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_auth);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_deinit);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_dev_rem);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_dma_map);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_stats_get);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_set_key);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_deauth);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_init);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_start_xmit);
EXPORT_SYMBOL_GPL(hal_rpu_reg_read);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_dev_init);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_get_channel);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_deinit);
EXPORT_SYMBOL_GPL(hal_rpu_reg_write);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_get_station);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_del_key);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_set_power_save);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_mem_cpy);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_ver_get);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_write_reg32);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_intr_reg);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_iomem_mmap);
EXPORT_SYMBOL_GPL(nrf_wifi_fmac_vif_update_if_type);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_dev_add);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_init);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_scan);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_host_map_get);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_bus_pcie_dev_intr_unreg);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_set_mcast_addr);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_del_vif);
EXPORT_SYMBOL_GPL(nrf_wifi_osal_log_err);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_disassoc);
EXPORT_SYMBOL_GPL(nrf_wifi_sys_fmac_add_key);

static int __init nrf_wifi_osal_mod_init(void) {
	return 0;
}

static void __exit nrf_wifi_osal_mod_deinit(void) {
}

module_init(nrf_wifi_osal_mod_init);
module_exit(nrf_wifi_osal_mod_deinit);

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nordic Semiconductor ASA");
MODULE_DESCRIPTION("nrf_wifi_osal: OS abstraction layer for nRF Wi-Fi driver");
MODULE_VERSION("1.0");