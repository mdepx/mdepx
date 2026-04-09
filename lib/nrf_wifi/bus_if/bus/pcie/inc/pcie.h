/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @brief File containing declarations for the
 * PCIe bus Layer of the Wi-Fi driver.
 */

#include "bal_structs.h"

/* TODO: Move these defines to Platform layer */
#define NRF_WIFI_PCIE_DRV_NAME "nrf_wifi_pcie"
#define NRF_WIFI_PCI_VENDOR_ID 0x0700
#define NRF_WIFI_PCI_SUB_VENDOR_ID (~0)
#define NRF_WIFI_PCI_DEVICE_ID (~0)
#define NRF_WIFI_PCI_SUB_DEVICE_ID (~0)

/**
 * struct nrf_wifi_bus_pcie_priv - Structure to hold context information for the PCIe bus.
 * @opriv: Pointer to the OSAL context.
 * @mmap_dev_addr_base: Pointer to the base of the device i/o address memory
 *                      mapped into the host memory.
 * @mmap_dev_addr_len: Length of the device i/o address memory mapped into the
 *                     host memory.
 * @irq: Interrupt line used by the PCIe device.
 * @irq_flags: Interrupts flag to indicate whether interrupt is shared etc.
 *
 * This structure maintains the context information necessary for the operation
 * of the PCIe bus. Some of the elements of the structure need to be initialized
 * during the initialization of the PCIe bus while others need to be kept
 * updated over the duration of the PCIe bus operation.
 */
struct nrf_wifi_bus_pcie_priv {
	struct nrf_wifi_osal_priv *opriv;
	void *os_pcie_priv;

	int (*intr_callbk_fn)(void *bal_dev_ctx);

	/* TODO: See if this can be removed by getting the information from PAL */
	struct nrf_wifi_bal_cfg_params cfg_params;
};


struct nrf_wifi_bus_pcie_dev_ctx {
	struct nrf_wifi_bus_pcie_priv *pcie_priv;
	void *bal_dev_ctx;
	void *os_pcie_dev_ctx;

	void *iomem_addr_base;
	unsigned long addr_pktram_base;
#ifdef SOC_WEZEN
#ifdef INLINE_RX
	unsigned long addr_hostram_base_inline_rx;
#endif
#endif
#ifdef DEBUG_MODE_SUPPORT
#ifdef DCR14_VALIDATE
	bool bus_access_rec_enab;
	unsigned int bus_access_cnt;
#endif /* DCR14_VALIDATE */
#endif	
};
