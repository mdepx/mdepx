/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @brief Common structures and definitions.
 */

#ifndef __RPU_UMAC_STATS_H__
#define __RPU_UMAC_STATS_H__

#include "rpu_stats_common.h"

/**
* @brief MAC debug variables.
*/
struct rpu_stat_from_mem rpu_umac_stats[] = {
	{"tx_cmd", 0x8008bd9c },
	{"tx_cmds_currently_in_use", 0x8008bdb0 },
	{"tx_done_events_send_to_host", 0x8008bdb4 },
	{"tx_cmd_to_lmac", 0x8008bdec },
	{"tx_dones_from_lmac", 0x8008bdf0 },
	{"total_cmds_to_lmac", 0x8008bdf4 },
	{"cmd_processing", 0x80132000 },
	{"lmac_events", 0x80080388 },
	{"rx_events", 0x8008038c },
	{"current_refill_gap", 0x8008039c },
	{"umac_consumed_pkts", 0x800803a8 },
	{"host_consumed_pkts", 0x800803ac },
	{"rx_mbox_post", 0x800803b0 },
	{"rx_mbox_receive", 0x800803b4 },
	{"cmd_trigger_scan", 0x8008bedc },
	{"event_scan_done", 0x8008bee0 },
	{"umac_scan_req", 0x8008bee8 },
	{"umac_scan_complete", 0x8008beec },
	{"cmd_set_ifflags", 0x8008bf3c },
	{"cmd_set_ifflags_done", 0x8008bf40 },
	{"command_cnt", 0x8008be24 },
	{"send_rx_buffs_to_host", 0x8008be2c },
	{"event_node_alloc_fail", 0x8008be3c },
	{"hpqm_event_pop_fail", 0x8008be40 },
	{"total_events_to_host", 0x8008be48 },
	{"cmd_init", 0x8008bec4 },
	{"event_init_done", 0x80098120 },
	{"null_skb_pointer_from_lmac", 0x8009811c },
};
 
/**
* @brief Raw TX RX debug variables.
*/
struct rpu_stat_from_mem rpu_rawtx_stats[] = {
	{"raw_tx_from_host", 0x800981e8 },
	{"raw_tx_to_lmac", 0x800981ec },
	{"raw_tx_dones_from_lmac", 0x800981f0 },
	{"raw_tx_dones_to_host", 0x800981f4 },
	{"total_rx_pkts_from_lmac", 0x800981f8 },
	{"raw_tx_dones_from_lmac", 0x800981f0 },
	{"total_raw_rx_pkts_from_lmac", 0x800981fc },
	{"valid_raw_rx_pkts_from_lmac", 0x80098200 },
	{"raw_tx_dones_from_lmac", 0x800981f0 },
	{"raw_rx_pkts_to_host", 0x80098204 },
};

/**
 * @brief Global array to hold all debug variable arrays and their sizes.
 */
struct rpu_stat_global rpu_all_umac_stats[] = {
	{"umac", rpu_umac_stats},
	{"rawtx", rpu_rawtx_stats},
	/* End of the list */
	{"", NULL}
};
 
#endif /* __RPU_UMAC_STATS_H__ */
