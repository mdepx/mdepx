/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @brief Implements PCIe bus ops to initialize and read/write on a PCIe bus
 */
#include "bal_structs.h"
#include "pcie.h"
#include "common/pal.h"

#define NRF_WIFI_PCIE_DEV_NAME "nrfwifi0"

int nrf_wifi_bus_pcie_irq_handler(void *data)
{
	struct nrf_wifi_bus_pcie_dev_ctx *dev_ctx = NULL;
	struct nrf_wifi_bus_pcie_priv *pcie_priv = NULL;
	int ret = 0;

	dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)data;
	pcie_priv = dev_ctx->pcie_priv;

	ret = pcie_priv->intr_callbk_fn(dev_ctx->bal_dev_ctx);

	return ret;
}

void *nrf_wifi_bus_pcie_dev_add(void *bus_priv,
								void *bal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_bus_pcie_priv *pcie_priv = NULL;
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	struct nrf_wifi_osal_host_map host_map;

	pcie_priv = bus_priv;

	pcie_dev_ctx = nrf_wifi_osal_mem_zalloc(
		sizeof(*pcie_dev_ctx));

	if (!pcie_dev_ctx)
	{
		nrf_wifi_osal_log_err(
			"%s: Unable to allocate pcie_dev_ctx", __func__);
		goto out;
	}

	pcie_dev_ctx->pcie_priv = pcie_priv;
	pcie_dev_ctx->bal_dev_ctx = bal_dev_ctx;

	pcie_dev_ctx->os_pcie_dev_ctx = nrf_wifi_osal_bus_pcie_dev_add(
		pcie_priv->os_pcie_priv,
		pcie_dev_ctx);

	if (!pcie_dev_ctx->os_pcie_dev_ctx)
	{
		nrf_wifi_osal_log_err(
			"%s: nrf_wifi_osal_bus_pcie_dev_add failed", __func__);

		nrf_wifi_osal_mem_free(
			pcie_dev_ctx);

		pcie_dev_ctx = NULL;

		goto out;
	}

	nrf_wifi_osal_bus_pcie_dev_host_map_get(
		pcie_dev_ctx->os_pcie_dev_ctx,
		&host_map);

	pcie_dev_ctx->iomem_addr_base = nrf_wifi_osal_iomem_mmap(
		host_map.addr,
		host_map.size);

#ifdef INLINE_BB_MODE
	pcie_dev_ctx->addr_pktram_base = pcie_priv->cfg_params.addr_pktram_base;
#endif /* INLINE_BB_MODE */
#ifdef OFFLINE_MODE
	pcie_dev_ctx->addr_pktram_base = ((unsigned long)pcie_dev_ctx->iomem_addr_base +
									  pcie_priv->cfg_params.addr_pktram_base);
#endif /* OFFLINE_MODE */
#ifdef INLINE_RX
	pcie_dev_ctx->addr_hostram_base_inline_rx = pcie_priv->cfg_params.addr_hostram_base_inline_rx;
#endif

	status = nrf_wifi_osal_bus_pcie_dev_intr_reg(
		pcie_dev_ctx->os_pcie_dev_ctx,
		pcie_dev_ctx,
		&nrf_wifi_bus_pcie_irq_handler);

	if (status != NRF_WIFI_STATUS_SUCCESS)
	{
		nrf_wifi_osal_log_err(
			"%s: Unable to register PCIe interrupt to the OS",
			__func__);

		nrf_wifi_osal_iomem_unmap(
			pcie_dev_ctx->iomem_addr_base);

		nrf_wifi_osal_bus_pcie_dev_rem(
			pcie_dev_ctx->os_pcie_dev_ctx);

		nrf_wifi_osal_mem_free(
			pcie_dev_ctx);

		pcie_dev_ctx = NULL;

		goto out;
	}

out:
	return pcie_dev_ctx;
}

void nrf_wifi_bus_pcie_dev_rem(void *bus_dev_ctx)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;

	pcie_dev_ctx = bus_dev_ctx;

	nrf_wifi_osal_iomem_unmap(
		pcie_dev_ctx->iomem_addr_base);

	nrf_wifi_osal_bus_pcie_dev_intr_unreg(
		pcie_dev_ctx->os_pcie_dev_ctx);

	nrf_wifi_osal_bus_pcie_dev_rem(
		pcie_dev_ctx->os_pcie_dev_ctx);

	nrf_wifi_osal_mem_free(
		pcie_dev_ctx);
}

enum nrf_wifi_status nrf_wifi_bus_pcie_dev_init(void *bus_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;

	pcie_dev_ctx = bus_dev_ctx;

	status = nrf_wifi_osal_bus_pcie_dev_init(
		pcie_dev_ctx->os_pcie_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS)
	{
		nrf_wifi_osal_log_err(
			"%s: nrf_wifi_osal_pcie_dev_init failed", __func__);

		goto out;
	}
out:
	return status;
}

void nrf_wifi_bus_pcie_dev_deinit(void *bus_dev_ctx)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;

	pcie_dev_ctx = bus_dev_ctx;

	nrf_wifi_osal_bus_pcie_dev_deinit(
		pcie_dev_ctx->os_pcie_dev_ctx);
}

void *nrf_wifi_bus_pcie_init(void *params,
							 int (*intr_callbk_fn)(void *bal_dev_ctx))
{
	struct nrf_wifi_bus_pcie_priv *pcie_priv = NULL;

	pcie_priv = nrf_wifi_osal_mem_zalloc(
		sizeof(*pcie_priv));

	if (!pcie_priv)
	{
		nrf_wifi_osal_log_err(
			"%s: Unable to allocate memory for pcie_priv",
			__func__);
		goto out;
	}

	nrf_wifi_osal_mem_cpy(
		&pcie_priv->cfg_params,
		params,
		sizeof(pcie_priv->cfg_params));

	pcie_priv->intr_callbk_fn = intr_callbk_fn;

	pcie_priv->os_pcie_priv = nrf_wifi_osal_bus_pcie_init(
		NRF_WIFI_PCIE_DRV_NAME,
		NRF_WIFI_PCI_VENDOR_ID,
		NRF_WIFI_PCI_SUB_VENDOR_ID,
		NRF_WIFI_PCI_DEVICE_ID,
		NRF_WIFI_PCI_SUB_DEVICE_ID);

	if (!pcie_priv->os_pcie_priv)
	{
		nrf_wifi_osal_log_err(
			"%s: Unable to register PCIe driver",
			__func__);

		nrf_wifi_osal_mem_free(
			pcie_priv);

		pcie_priv = NULL;

		goto out;
	}
out:
	return pcie_priv;
}

void nrf_wifi_bus_pcie_deinit(void *bus_priv)
{
	struct nrf_wifi_bus_pcie_priv *pcie_priv = NULL;

	pcie_priv = bus_priv;

	nrf_wifi_osal_bus_pcie_deinit(
		pcie_priv->os_pcie_priv);

	nrf_wifi_osal_mem_free(
		pcie_priv);
}

unsigned int nrf_wifi_bus_pcie_read_word(void *dev_ctx,
										 unsigned long addr_offset)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	void *mmap_addr = NULL;
	unsigned int val = 0;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

	mmap_addr = pcie_dev_ctx->iomem_addr_base + addr_offset;

	val = nrf_wifi_osal_iomem_read_reg32(
		mmap_addr);

	return val;
}

void nrf_wifi_bus_pcie_write_word(void *dev_ctx,
								  unsigned long addr_offset,
								  unsigned int val)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	void *mmap_addr = NULL;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

	mmap_addr = pcie_dev_ctx->iomem_addr_base + addr_offset;

	nrf_wifi_osal_iomem_write_reg32(
		mmap_addr,
		val);
}

void nrf_wifi_bus_pcie_read_block(void *dev_ctx,
								  void *dest_addr,
								  unsigned long src_addr_offset,
								  size_t len)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	void *mmap_addr = NULL;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

	mmap_addr = pcie_dev_ctx->iomem_addr_base + src_addr_offset;

	nrf_wifi_osal_iomem_cpy_from(
		dest_addr,
		mmap_addr,
		len);
}

void nrf_wifi_bus_pcie_write_block(void *dev_ctx,
								   unsigned long dest_addr_offset,
								   const void *src_addr,
								   size_t len)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	void *mmap_addr = NULL;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

	mmap_addr = pcie_dev_ctx->iomem_addr_base + dest_addr_offset;

	nrf_wifi_osal_iomem_cpy_to(
		mmap_addr,
		src_addr,
		len);
}
#ifdef SOC_WEZEN
#ifdef INLINE_RX
unsigned long nrf_wifi_bus_pcie_dma_map_inline_rx(void *dev_ctx,
												  unsigned long virt_addr,
												  size_t len,
												  enum nrf_wifi_osal_dma_dir dma_dir)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	unsigned long phy_addr = 0;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;
	phy_addr = SOC_HOST_DATA_RAM_BASE + (virt_addr - pcie_dev_ctx->addr_hostram_base_inline_rx);
	return phy_addr;
}
unsigned long nrf_wifi_bus_pcie_dma_unmap_inline_rx(void *dev_ctx,
													unsigned long phy_addr,
													size_t len,
													enum nrf_wifi_osal_dma_dir dma_dir)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	unsigned long virt_addr = 0;
	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;
	virt_addr = pcie_dev_ctx->addr_hostram_base_inline_rx + (phy_addr - SOC_HOST_DATA_RAM_BASE);
	return virt_addr;
}
#endif
#endif

unsigned long nrf_wifi_bus_pcie_dma_map(void *dev_ctx,
										unsigned long virt_addr,
										size_t len,
										enum nrf_wifi_osal_dma_dir dma_dir)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	unsigned long phy_addr = 0;

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

#ifdef INLINE_MODE
	phy_addr = (unsigned long)nrf_wifi_osal_bus_pcie_dev_dma_map(
		pcie_dev_ctx->os_pcie_dev_ctx,
		(void *)virt_addr,
		len,
		dma_dir);
#endif /* INLINE_MODE */
#ifdef INLINE_BB_MODE
	phy_addr = SOC_HOST_PKTRAM_BASE + (virt_addr - pcie_dev_ctx->addr_pktram_base);
#endif /* INLINE_BB_MODE */
#ifdef OFFLINE_MODE
	phy_addr = (unsigned long)pcie_dev_ctx->iomem_addr_base + (virt_addr - pcie_dev_ctx->addr_pktram_base);
#endif /* OFFLINE_MODE */

	return phy_addr;
}

unsigned long nrf_wifi_bus_pcie_dma_unmap(void *dev_ctx,
										  unsigned long phy_addr,
										  size_t len,
										  enum nrf_wifi_osal_dma_dir dma_dir)
{
	struct nrf_wifi_bus_pcie_dev_ctx *pcie_dev_ctx = NULL;
	unsigned long virt_addr = 0;
#ifdef OFFLINE_MODE
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
#endif /* OFFLINE_MODE */

	pcie_dev_ctx = (struct nrf_wifi_bus_pcie_dev_ctx *)dev_ctx;

#ifdef INLINE_MODE
	nrf_wifi_osal_bus_pcie_dev_dma_unmap(
		pcie_dev_ctx->os_pcie_dev_ctx,
		(void *)phy_addr,
		len,
		dma_dir);
#endif /* INLINE_MODE */
#ifdef INLINE_BB_MODE
	virt_addr = pcie_dev_ctx->addr_pktram_base + (phy_addr - SOC_HOST_PKTRAM_BASE);
#endif /* INLINE_BB_MODE */
#ifdef OFFLINE_MODE
#ifdef SOC_WEZEN
	status = pal_rpu_addr_offset_get(
		RPU_ADDR_DATA_RAM_START + phy_addr,
		&virt_addr,
		RPU_ADDR_DATA_RAM_START);
#else
	status = pal_rpu_addr_offset_get(
		RPU_ADDR_PKTRAM_START + phy_addr,
		&virt_addr,
		RPU_ADDR_PKTRAM_START);
#endif
	if (status != NRF_WIFI_STATUS_SUCCESS)
		nrf_wifi_osal_log_err(
			"%s: pal_rpu_addr_offset_get failed", __func__);
#endif /* OFFLINE_MODE */

	return virt_addr;
}

#ifdef NRF_WIFI_LOW_POWER
void nrf_wifi_bus_pcie_rpu_ps_sleep(void *bus_dev_ctx)
{
	unsigned long reg_addr = 0;
	unsigned int reg_val = 0;

	reg_addr = pal_rpu_ps_ctrl_reg_addr_get();

	reg_val = nrf_wifi_bus_pcie_read_word(bus_dev_ctx,
										  reg_addr);

	reg_val &= (~(1 << RPU_REG_BIT_PS_CTRL));

	nrf_wifi_bus_pcie_write_word(bus_dev_ctx,
								 reg_addr,
								 reg_val);
}

void nrf_wifi_bus_pcie_rpu_ps_wake(void *bus_dev_ctx)
{
	unsigned long reg_addr = 0;
	unsigned int reg_val = 0;

	reg_addr = pal_rpu_ps_ctrl_reg_addr_get();

	reg_val = (1 << RPU_REG_BIT_PS_CTRL);

	nrf_wifi_bal_write_word(bus_dev_ctx,
							reg_addr,
							reg_val);
}

int nrf_wifi_bus_pcie_rpu_ps_status(void *bus_dev_ctx)
{
	unsigned long reg_addr = 0;

	reg_addr = pal_rpu_ps_ctrl_reg_addr_get();

	return nrf_wifi_bus_pcie_read_word(bus_dev_ctx,
									   reg_addr);
}
#endif /* NRF_WIFI_LOW_POWER */

struct nrf_wifi_bal_ops nrf_wifi_bus_pcie_ops = {
	.init = &nrf_wifi_bus_pcie_init,
	.deinit = &nrf_wifi_bus_pcie_deinit,
	.dev_add = &nrf_wifi_bus_pcie_dev_add,
	.dev_rem = &nrf_wifi_bus_pcie_dev_rem,
	.dev_init = &nrf_wifi_bus_pcie_dev_init,
	.dev_deinit = &nrf_wifi_bus_pcie_dev_deinit,
	.read_word = &nrf_wifi_bus_pcie_read_word,
	.write_word = &nrf_wifi_bus_pcie_write_word,
	.read_block = &nrf_wifi_bus_pcie_read_block,
	.write_block = &nrf_wifi_bus_pcie_write_block,
	.dma_map = &nrf_wifi_bus_pcie_dma_map,
	.dma_unmap = &nrf_wifi_bus_pcie_dma_unmap,
#ifdef SOC_WEZEN
#ifdef INLINE_RX
	.dma_map_inline_rx = &nrf_wifi_bus_pcie_dma_map_inline_rx,
	.dma_unmap_inline_rx = &nrf_wifi_bus_pcie_dma_unmap_inline_rx,
#endif
#endif
#ifdef NRF_WIFI_LOW_POWER
	.rpu_ps_sleep = &nrf_wifi_bus_pcie_rpu_ps_sleep,
	.rpu_ps_wake = &nrf_wifi_bus_pcie_rpu_ps_wake,
	.rpu_ps_status = &nrf_wifi_bus_pcie_rpu_ps_status,
#endif /* NRF_WIFI_LOW_POWER */
};

struct nrf_wifi_bal_ops *get_bus_ops(void)
{
	return &nrf_wifi_bus_pcie_ops;
}
