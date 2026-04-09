/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief File containing API definitions for the
 * HAL Layer of the Wi-Fi driver in the system mode of operation.
 */

#include "queue.h"
#include "common/hal_structs_common.h"
#include "common/hal_reg.h"
#include "common/hal_mem.h"
#include "common/hal_common.h"
#include "common/hal_interrupt.h"
#include "common/pal.h"
#include "system/hal_api.h"

static enum nrf_wifi_status
nrf_wifi_sys_hal_rpu_pktram_buf_map_init(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int pool_idx = 0;

	status = pal_rpu_addr_offset_get(RPU_MEM_PKT_BASE,
					 &hal_dev_ctx->addr_rpu_pktram_base,
					 hal_dev_ctx->curr_proc);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: pal_rpu_addr_offset_get failed",
				      __func__);
		goto out;
	}

	hal_dev_ctx->addr_rpu_pktram_base_tx = hal_dev_ctx->addr_rpu_pktram_base;

	hal_dev_ctx->addr_rpu_pktram_base_rx_pool[0] =
		 (hal_dev_ctx->addr_rpu_pktram_base + RPU_PKTRAM_SIZE) -
		 (NRF70_RX_NUM_BUFS * NRF70_RX_MAX_DATA_SIZE);

	for (pool_idx = 1; pool_idx < MAX_NUM_OF_RX_QUEUES; pool_idx++) {
		hal_dev_ctx->addr_rpu_pktram_base_rx_pool[pool_idx] =
			hal_dev_ctx->addr_rpu_pktram_base_rx_pool[pool_idx - 1] +
			(hal_dev_ctx->hpriv->cfg_params.rx_buf_pool[pool_idx - 1].num_bufs *
			 hal_dev_ctx->hpriv->cfg_params.rx_buf_pool[pool_idx - 1].buf_sz);
	}

	status = NRF_WIFI_STATUS_SUCCESS;
out:
	return status;
}

static enum nrf_wifi_status hal_rpu_recovery(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	if (!hal_dev_ctx->hpriv->rpu_recovery_callbk_fn) {
		nrf_wifi_osal_log_dbg("%s: RPU recovery callback not registered",
				      __func__);
		goto out;
	}

	status = hal_dev_ctx->hpriv->rpu_recovery_callbk_fn(hal_dev_ctx->mac_dev_ctx, NULL, 0);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: RPU recovery failed",
				      __func__);
		goto out;
	}

out:
	return status;
}

static void recovery_tasklet_fn(unsigned long data)
{
	struct nrf_wifi_hal_dev_ctx *hal_dev_ctx = NULL;
	unsigned long flags = 0;

	hal_dev_ctx = (struct nrf_wifi_hal_dev_ctx *)data;
	if (!hal_dev_ctx) {
		nrf_wifi_osal_log_err("%s: Invalid hal_dev_ctx",
				      __func__);
		return;
	}

	nrf_wifi_osal_spinlock_irq_take(hal_dev_ctx->lock_recovery,
					&flags);
	hal_rpu_recovery(hal_dev_ctx);
	nrf_wifi_osal_spinlock_irq_rel(hal_dev_ctx->lock_recovery,
				       &flags);
}


unsigned long nrf_wifi_sys_hal_buf_map_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					  unsigned long buf,
					  unsigned int buf_len,
					  unsigned int pool_id,
					  unsigned int buf_id)
{
	struct nrf_wifi_hal_buf_map_info *rx_buf_info = NULL;
	unsigned long addr_to_map = 0;
	unsigned long bounce_buf_addr = 0;
	unsigned long rpu_addr = 0;

	if (!hal_dev_ctx || !hal_dev_ctx->rx_buf_info[pool_id]) {
		nrf_wifi_osal_log_err("%s: Invalid parameters",
				      __func__);
		goto out;
	}

	rx_buf_info = &hal_dev_ctx->rx_buf_info[pool_id][buf_id];

	if (rx_buf_info->mapped) {
		nrf_wifi_osal_log_err("%s: Called for already mapped RX buffer",
				      __func__);
		goto out;
	}

	rx_buf_info->virt_addr = buf;
	rx_buf_info->buf_len = buf_len;

	if (buf_len != hal_dev_ctx->hpriv->cfg_params.rx_buf_pool[pool_id].buf_sz) {
		nrf_wifi_osal_log_err("%s: Invalid buf_len (%d) for pool_id (%d)",
				      __func__,
				      buf_len,
				      pool_id);
		goto out;
	}

	bounce_buf_addr = hal_dev_ctx->addr_rpu_pktram_base_rx_pool[pool_id] +
		(buf_id * buf_len);

	rpu_addr = RPU_MEM_PKT_BASE + (bounce_buf_addr - hal_dev_ctx->addr_rpu_pktram_base);

	hal_rpu_mem_write(hal_dev_ctx,
			  (unsigned int)rpu_addr,
			  (void *)buf,
			  hal_dev_ctx->hpriv->cfg_params.rx_buf_headroom_sz);

	addr_to_map = bounce_buf_addr + hal_dev_ctx->hpriv->cfg_params.rx_buf_headroom_sz;

	rx_buf_info->phy_addr = nrf_wifi_bal_dma_map(hal_dev_ctx->bal_dev_ctx,
						     addr_to_map,
						     buf_len,
						     NRF_WIFI_OSAL_DMA_DIR_FROM_DEV);

	if (!rx_buf_info->phy_addr) {
		nrf_wifi_osal_log_err("%s: DMA map failed",
				      __func__);
		goto out;
	}

out:
	if (rx_buf_info->phy_addr) {
		rx_buf_info->mapped = true;
	}

	return rx_buf_info->phy_addr;
}


unsigned long nrf_wifi_sys_hal_buf_unmap_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					    unsigned int data_len,
					    unsigned int pool_id,
					    unsigned int buf_id)
{
	struct nrf_wifi_hal_buf_map_info *rx_buf_info = NULL;
	unsigned long unmapped_addr = 0;
	unsigned long virt_addr = 0;
	unsigned long rpu_addr = 0;

	rx_buf_info = &hal_dev_ctx->rx_buf_info[pool_id][buf_id];

	if (!rx_buf_info->mapped) {
		nrf_wifi_osal_log_err("%s: Called for unmapped RX buffer",
				      __func__);
		goto out;
	}

	unmapped_addr = nrf_wifi_bal_dma_unmap(hal_dev_ctx->bal_dev_ctx,
					       rx_buf_info->phy_addr,
					       rx_buf_info->buf_len,
					       NRF_WIFI_OSAL_DMA_DIR_FROM_DEV);

	rpu_addr = RPU_MEM_PKT_BASE + (unmapped_addr - hal_dev_ctx->addr_rpu_pktram_base);

	if (data_len) {
		if (!unmapped_addr) {
			nrf_wifi_osal_log_err("%s: DMA unmap failed",
					      __func__);
			goto out;
		}

		hal_rpu_mem_read(hal_dev_ctx,
				 (void *)(rx_buf_info->virt_addr +
					  hal_dev_ctx->hpriv->cfg_params.rx_buf_headroom_sz),
				 (unsigned int)rpu_addr,
				 data_len);
	}

	virt_addr = rx_buf_info->virt_addr;

	nrf_wifi_osal_mem_set(rx_buf_info,
			      0,
			      sizeof(*rx_buf_info));
out:
	return virt_addr;
}


unsigned long nrf_wifi_sys_hal_buf_map_tx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					  unsigned long buf,
					  unsigned int buf_len,
					  unsigned int desc_id,
					  unsigned int token,
					  unsigned int buf_indx)
{
	struct nrf_wifi_hal_buf_map_info *tx_buf_info = NULL;
	unsigned long addr_to_map = 0;
	unsigned long bounce_buf_addr = 0;
	unsigned long tx_token_base_addr = hal_dev_ctx->addr_rpu_pktram_base_tx +
		(token * hal_dev_ctx->hpriv->cfg_params.max_ampdu_len_per_token);
	unsigned long rpu_addr = 0;

	tx_buf_info = &hal_dev_ctx->tx_buf_info[desc_id];

	if (tx_buf_info->mapped) {
		nrf_wifi_osal_log_err("%s: Called for already mapped TX buffer",
				      __func__);
		goto out;
	}

	tx_buf_info->virt_addr = buf;

	if (buf_len > (hal_dev_ctx->hpriv->cfg_params.max_tx_frm_sz -
		       hal_dev_ctx->hpriv->cfg_params.tx_buf_headroom_sz)) {
		nrf_wifi_osal_log_err("%s: Invalid TX buf_len (%d) for (%d)",
				      __func__,
				      buf_len,
				      desc_id);
		goto out;
	}

	if (buf_indx == 0) {
		hal_dev_ctx->tx_frame_offset = tx_token_base_addr;
	}

	bounce_buf_addr = hal_dev_ctx->tx_frame_offset;

	/* Align bounce buffer and buffer length to 4-byte boundary */
	bounce_buf_addr = (bounce_buf_addr + 3) & ~3;
	buf_len = (buf_len + 3) & ~3;

	hal_dev_ctx->tx_frame_offset += (bounce_buf_addr - hal_dev_ctx->tx_frame_offset) +
		buf_len + hal_dev_ctx->hpriv->cfg_params.tx_buf_headroom_sz;

	rpu_addr = RPU_MEM_PKT_BASE + (bounce_buf_addr - hal_dev_ctx->addr_rpu_pktram_base);

	nrf_wifi_osal_log_dbg("%s: bounce_buf_addr: 0x%lx, rpu_addr: 0x%lx, buf_len: %d off:%d",
	       __func__,
	       bounce_buf_addr,
	       rpu_addr,
	       buf_len,
	       hal_dev_ctx->tx_frame_offset);

	hal_rpu_mem_write(hal_dev_ctx,
			  (unsigned int)rpu_addr,
			  (void *)buf,
			  buf_len);

	addr_to_map = bounce_buf_addr;

	tx_buf_info->phy_addr = nrf_wifi_bal_dma_map(hal_dev_ctx->bal_dev_ctx,
						     addr_to_map,
						     buf_len,
						     NRF_WIFI_OSAL_DMA_DIR_TO_DEV);

	if (!tx_buf_info->phy_addr) {
		nrf_wifi_osal_log_err("%s: DMA map failed",
				      __func__);
		goto out;
	}
	tx_buf_info->buf_len = buf_len;

out:
	if (tx_buf_info->phy_addr) {
		tx_buf_info->mapped = true;
	}

	return tx_buf_info->phy_addr;
}


unsigned long nrf_wifi_sys_hal_buf_unmap_tx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					    unsigned int desc_id)
{
	struct nrf_wifi_hal_buf_map_info *tx_buf_info = NULL;
	unsigned long unmapped_addr = 0;
	unsigned long virt_addr = 0;

	tx_buf_info = &hal_dev_ctx->tx_buf_info[desc_id];

	if (!tx_buf_info->mapped) {
		nrf_wifi_osal_log_err("%s: Called for unmapped TX buffer",
				      __func__);
		goto out;
	}

	unmapped_addr = nrf_wifi_bal_dma_unmap(hal_dev_ctx->bal_dev_ctx,
					       tx_buf_info->phy_addr,
					       tx_buf_info->buf_len,
					       NRF_WIFI_OSAL_DMA_DIR_TO_DEV);

	if (!unmapped_addr) {
		nrf_wifi_osal_log_err("%s: DMA unmap failed",
				      __func__);
		goto out;
	}

	virt_addr = tx_buf_info->virt_addr;

	nrf_wifi_osal_mem_set(tx_buf_info,
			      0,
			      sizeof(*tx_buf_info));
out:
	return virt_addr;
}

enum nrf_wifi_status nrf_wifi_sys_hal_data_cmd_send(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
						    enum NRF_WIFI_HAL_MSG_TYPE cmd_type,
						    void *cmd,
						    unsigned int cmd_size,
						    unsigned int desc_id,
						    unsigned int pool_id)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	unsigned int addr_base = 0;
	unsigned int max_cmd_size = 0;
	unsigned int host_addr = 0;
	unsigned int addr = 0;

	nrf_wifi_osal_spinlock_take(hal_dev_ctx->lock_hal);
	if (cmd_type == NRF_WIFI_HAL_MSG_TYPE_CMD_DATA_RX) {
		addr_base = hal_dev_ctx->rpu_info.rx_cmd_base;
		max_cmd_size = RPU_DATA_CMD_SIZE_MAX_RX;
	} else if (cmd_type == NRF_WIFI_HAL_MSG_TYPE_CMD_DATA_TX) {
		addr_base = hal_dev_ctx->rpu_info.tx_cmd_base;
		max_cmd_size = RPU_DATA_CMD_SIZE_MAX_TX;
	} else {
		nrf_wifi_osal_log_err("%s: Invalid data command type %d",
				      __func__,
				      cmd_type);
	}

	addr = addr_base + (max_cmd_size * desc_id);
	host_addr = addr;

	/* This is a indrect write to core memory */
	if (cmd_type == NRF_WIFI_HAL_MSG_TYPE_CMD_DATA_RX) {
		host_addr &= RPU_ADDR_MASK_OFFSET;
		host_addr |= RPU_MCU_CORE_INDIRECT_BASE;
	}

	/* Copy the information to the suggested address */
	status = hal_rpu_mem_write(hal_dev_ctx,
				   host_addr,
				   cmd,
				   cmd_size);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Copying data cmd(%d) to RPU failed",
				      __func__,
				      cmd_type);
		goto out;
	}

	/* Post the updated information to the RPU */
	status = hal_rpu_msg_post(hal_dev_ctx,
				  cmd_type,
				  pool_id,
				  addr);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Posting RX buf info to RPU failed",
				      __func__);
		goto out;
	}
out:
	nrf_wifi_osal_spinlock_rel(hal_dev_ctx->lock_hal);


	return status;
}

static void event_tasklet_fn(unsigned long data)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_hal_dev_ctx *hal_dev_ctx = NULL;
	unsigned long flags = 0;

	hal_dev_ctx = (struct nrf_wifi_hal_dev_ctx *)data;

	nrf_wifi_osal_spinlock_irq_take(hal_dev_ctx->lock_rx,
					&flags);

	if (hal_dev_ctx->hal_status != NRF_WIFI_HAL_STATUS_ENABLED) {
		/* Ignore the interrupt if the HAL is not enabled */
		status = NRF_WIFI_STATUS_SUCCESS;
		goto out;
	}

	status = hal_rpu_eventq_process(hal_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Event queue processing failed",
				      __func__);
	}

out:
	nrf_wifi_osal_spinlock_irq_rel(hal_dev_ctx->lock_rx,
				       &flags);
}

struct nrf_wifi_hal_dev_ctx *nrf_wifi_sys_hal_dev_add(struct nrf_wifi_hal_priv *hpriv,
						      void *mac_dev_ctx)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;
	struct nrf_wifi_hal_dev_ctx *hal_dev_ctx = NULL;
	unsigned int i = 0;
	unsigned int num_rx_bufs = 0;
	unsigned int size = 0;

	hal_dev_ctx = nrf_wifi_osal_mem_zalloc(sizeof(*hal_dev_ctx));

	if (!hal_dev_ctx) {
		nrf_wifi_osal_log_err("%s: Unable to allocate hal_dev_ctx",
				      __func__);
		goto err;
	}

	hal_dev_ctx->hpriv = hpriv;
	hal_dev_ctx->mac_dev_ctx = mac_dev_ctx;
	hal_dev_ctx->idx = hpriv->num_devs++;
	hal_dev_ctx->num_cmds = RPU_CMD_START_MAGIC;

	hal_dev_ctx->cmd_q = nrf_wifi_utils_ctrl_q_alloc();

	if (!hal_dev_ctx->cmd_q) {
		nrf_wifi_osal_log_err("%s: Unable to allocate command queue",
				      __func__);
		goto hal_dev_free;
	}

	hal_dev_ctx->event_q = nrf_wifi_utils_ctrl_q_alloc();

	if (!hal_dev_ctx->event_q) {
		nrf_wifi_osal_log_err("%s: Unable to allocate event queue",
				      __func__);
		goto cmd_q_free;
	}

	hal_dev_ctx->lock_hal = nrf_wifi_osal_spinlock_alloc();

	if (!hal_dev_ctx->lock_hal) {
		nrf_wifi_osal_log_err("%s: Unable to allocate HAL lock", __func__);
		hal_dev_ctx = NULL;
		goto event_q_free;
	}

	nrf_wifi_osal_spinlock_init(hal_dev_ctx->lock_hal);

	hal_dev_ctx->lock_rx = nrf_wifi_osal_spinlock_alloc();

	if (!hal_dev_ctx->lock_rx) {
		nrf_wifi_osal_log_err("%s: Unable to allocate HAL lock",
				      __func__);
		goto lock_hal_free;
	}

	nrf_wifi_osal_spinlock_init(hal_dev_ctx->lock_rx);

	hal_dev_ctx->event_tasklet = nrf_wifi_osal_tasklet_alloc(NRF_WIFI_TASKLET_TYPE_BH);

	if (!hal_dev_ctx->event_tasklet) {
		nrf_wifi_osal_log_err("%s: Unable to allocate event_tasklet",
				      __func__);
		goto lock_rx_free;
	}

	nrf_wifi_osal_tasklet_init(hal_dev_ctx->event_tasklet,
				   event_tasklet_fn,
				   (unsigned long)hal_dev_ctx);

	hal_dev_ctx->recovery_tasklet = nrf_wifi_osal_tasklet_alloc(NRF_WIFI_TASKLET_TYPE_BH);
	if (!hal_dev_ctx->recovery_tasklet) {
		nrf_wifi_osal_log_err("%s: Unable to allocate recovery_tasklet",
				      __func__);
		goto event_tasklet_free;
	}
	nrf_wifi_osal_tasklet_init(hal_dev_ctx->recovery_tasklet,
				   recovery_tasklet_fn,
				   (unsigned long)hal_dev_ctx);

	hal_dev_ctx->lock_recovery = nrf_wifi_osal_spinlock_alloc();
	if (!hal_dev_ctx->lock_recovery) {
		nrf_wifi_osal_log_err("%s: Unable to allocate recovery lock",
				      __func__);
		goto recovery_tasklet_free;
	}

	nrf_wifi_osal_spinlock_init(hal_dev_ctx->lock_recovery);

#ifdef NRF_WIFI_LOW_POWER
	status = hal_rpu_ps_init(hal_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: hal_rpu_ps_init failed",
				      __func__);
		goto lock_recovery_free;
	}
#endif /* NRF_WIFI_LOW_POWER */
	hal_dev_ctx->bal_dev_ctx = nrf_wifi_bal_dev_add(hpriv->bpriv,
							hal_dev_ctx);

	if (!hal_dev_ctx->bal_dev_ctx) {
		nrf_wifi_osal_log_err("%s: nrf_wifi_bal_dev_add failed",
				      __func__);
		goto lock_recovery_free;
	}
	status = hal_rpu_irq_enable(hal_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: hal_rpu_irq_enable failed",
				      __func__);
		goto bal_dev_free;
	}
	for (i = 0; i < MAX_NUM_OF_RX_QUEUES; i++) {
		num_rx_bufs = hal_dev_ctx->hpriv->cfg_params.rx_buf_pool[i].num_bufs;

		size = (num_rx_bufs * sizeof(struct nrf_wifi_hal_buf_map_info));

		hal_dev_ctx->rx_buf_info[i] = nrf_wifi_osal_mem_zalloc(size);

		if (!hal_dev_ctx->rx_buf_info[i]) {
			nrf_wifi_osal_log_err("%s: No space for RX buf info[%d]",
					      __func__,
					      i);
			goto bal_dev_free;
		}
	}
#ifdef NRF70_DATA_TX
	size = (hal_dev_ctx->hpriv->cfg_params.max_tx_frms *
		sizeof(struct nrf_wifi_hal_buf_map_info));

	hal_dev_ctx->tx_buf_info = nrf_wifi_osal_mem_zalloc(size);

	if (!hal_dev_ctx->tx_buf_info) {
		nrf_wifi_osal_log_err("%s: No space for TX buf info",
				      __func__);
		goto rx_buf_free;
	}
#endif /* NRF70_DATA_TX */
	status = nrf_wifi_sys_hal_rpu_pktram_buf_map_init(hal_dev_ctx);

	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Buffer map init failed",
				      __func__);
#ifdef NRF70_DATA_TX
		goto tx_buf_free;
#endif /* NRF70_DATA_TX */
	}
	return hal_dev_ctx;

#ifdef NRF70_DATA_TX
tx_buf_free:
	nrf_wifi_osal_mem_free(hal_dev_ctx->tx_buf_info);
	hal_dev_ctx->tx_buf_info = NULL;
rx_buf_free:

	for (i = 0; i < MAX_NUM_OF_RX_QUEUES; i++) {
		nrf_wifi_osal_mem_free(hal_dev_ctx->rx_buf_info[i]);
		hal_dev_ctx->rx_buf_info[i] = NULL;
	}
#endif /* NRF70_DATA_TX */
bal_dev_free:
	nrf_wifi_bal_dev_rem(hal_dev_ctx->bal_dev_ctx);
lock_recovery_free:
	nrf_wifi_osal_spinlock_free(hal_dev_ctx->lock_recovery);
recovery_tasklet_free:
	nrf_wifi_osal_tasklet_free(hal_dev_ctx->recovery_tasklet);
event_tasklet_free:
	nrf_wifi_osal_tasklet_free(hal_dev_ctx->event_tasklet);
lock_rx_free:
	nrf_wifi_osal_spinlock_free(hal_dev_ctx->lock_rx);
lock_hal_free:
	nrf_wifi_osal_spinlock_free(hal_dev_ctx->lock_hal);
event_q_free:
	nrf_wifi_utils_ctrl_q_free(hal_dev_ctx->event_q);
cmd_q_free:
	nrf_wifi_utils_ctrl_q_free(hal_dev_ctx->cmd_q);
hal_dev_free:
	nrf_wifi_osal_mem_free(hal_dev_ctx);
	hal_dev_ctx = NULL;
err:
	return NULL;
}

#ifdef NRF70_SR_COEX_SLEEP_CTRL_GPIO_CTRL
enum nrf_wifi_status nrf_wifi_hal_coex_config_sleep_ctrl_gpio_ctrl(
							struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
							unsigned int alt_swctrl1_function_bt_coex_status1,
							unsigned int invert_bt_coex_grant_output)
{
	enum nrf_wifi_status status = NRF_WIFI_STATUS_FAIL;

	unsigned int abs_sys_sleep_ctrl_gpio_ctrl = 0xA4002DC8;
	unsigned int sleep_ctrl_gpio_ctrl_read = 0;
	unsigned int sleep_ctrl_gpio_ctrl_write = 0;
	unsigned int alt_swctrl1_function_bt_coex_status1_mask = 0x00000040;
	unsigned int alt_swctrl1_function_bt_coex_status1_shift = 6;
	unsigned int invert_bt_coex_grant_output_mask = 0x00000200;
	unsigned int invert_bt_coex_grant_output_shift = 9;


	status = hal_rpu_reg_read(hal_dev_ctx,
				  &sleep_ctrl_gpio_ctrl_read,
				  abs_sys_sleep_ctrl_gpio_ctrl);
	sleep_ctrl_gpio_ctrl_write = sleep_ctrl_gpio_ctrl_read &
		(~(alt_swctrl1_function_bt_coex_status1_mask | invert_bt_coex_grant_output_mask));
	sleep_ctrl_gpio_ctrl_write |=
		((alt_swctrl1_function_bt_coex_status1 << alt_swctrl1_function_bt_coex_status1_shift) |
		(invert_bt_coex_grant_output << invert_bt_coex_grant_output_shift));

	hal_rpu_reg_write(hal_dev_ctx,
			  abs_sys_sleep_ctrl_gpio_ctrl,
			  sleep_ctrl_gpio_ctrl_write);

	status = hal_rpu_reg_read(hal_dev_ctx,
				  &sleep_ctrl_gpio_ctrl_read,
				  abs_sys_sleep_ctrl_gpio_ctrl);
	if (status != NRF_WIFI_STATUS_SUCCESS) {
		nrf_wifi_osal_log_err("%s: Failed to configure sleep control GPIO control registe",
					  __func__);
	}
	return status;
}
#endif /* NRF70_SR_COEX_SLEEP_CTRL_GPIO_CTRL */

void nrf_wifi_sys_hal_lock_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	unsigned long flags = 0;

	nrf_wifi_osal_spinlock_irq_take(hal_dev_ctx->lock_rx,
					&flags);
}

void nrf_wifi_sys_hal_unlock_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx)
{
	unsigned long flags = 0;

	nrf_wifi_osal_spinlock_irq_rel(hal_dev_ctx->lock_rx,
				       &flags);
}

#ifdef NRF_WIFI_RX_BUFF_PROG_UMAC
unsigned long nrf_wifi_hal_get_buf_map_rx(struct nrf_wifi_hal_dev_ctx *hal_dev_ctx,
					  unsigned int pool_id,
					  unsigned int buf_id)
{
	struct nrf_wifi_hal_buf_map_info *rx_buf_info = NULL;
	rx_buf_info = &hal_dev_ctx->rx_buf_info[pool_id][buf_id];

	if (rx_buf_info->mapped) {
		return rx_buf_info->phy_addr;
	} else {
		nrf_wifi_osal_log_err("%s: Rx buffer not mapped for pool_id = %d, buf_id=%d\n",
				      __func__,
				      pool_id,
				      buf_id);
	}

	return -1;
}
#endif /*NRF_WIFI_RX_BUFF_PROG_UMAC */
