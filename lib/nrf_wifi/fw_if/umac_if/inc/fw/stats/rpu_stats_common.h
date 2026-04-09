/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RPU_STATS_COMMON_H__
#define __RPU_STATS_COMMON_H__

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/**
 * @brief Common debug variables structure.
 */
struct rpu_stat_from_mem {
	const char *name;
	unsigned int addr;
};

struct rpu_stat_global {
	const char *name;
	struct rpu_stat_from_mem *stats;
};

#endif /* __RPU_STATS_COMMON_H__ */
