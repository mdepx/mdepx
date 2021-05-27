/******************************************************************************
 * Copyright (c) 2011 IBM Corporation
 * All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the BSD License
 * which accompanies this distribution, and is available at
 * http://www.opensource.org/licenses/bsd-license.php
 *
 * Contributors:
 *     IBM Corporation - initial implementation
 *****************************************************************************/

/*
 * Virtio block device definitions.
 * See Virtio Spec, Appendix D, for details
 */

#ifndef _VIRTIO_BLK_H
#define _VIRTIO_BLK_H

#include <stdint.h>

// As per VirtIO spec Version 1.1: 5.2.4 Device configuration layout
struct virtio_blk_cfg {
	uint64_t	capacity;
	uint32_t	size_max;
	uint32_t	seg_max;
	struct	virtio_blk_geometry {
		uint16_t	cylinders;
		uint8_t 	heads;
		uint8_t 	sectors;
	} geometry;
	uint32_t	blk_size;
	struct virtio_blk_topology {
		// # of logical blocks per physical block (log2)
		uint8_t physical_block_exp;
		// offset of first aligned logical block
		uint8_t alignment_offset;
		// suggested minimum I/O size in blocks
			uint16_t min_io_size;
		// optimal (suggested maximum) I/O size in blocks
		uint32_t opt_io_size;
	} topology;
	uint8_t writeback;
	uint8_t unused0[3];
	uint32_t max_discard_sectors;
	uint32_t max_discard_seg;
	uint32_t discard_sector_alignment;
	uint32_t max_write_zeroes_sectors;
	uint32_t max_write_zeroes_seg;
	uint8_t write_zeroes_may_unmap;
	uint8_t unused1[3];
} __attribute__((packed));

/* Block request */
struct virtio_blk_req {
	uint32_t  type;
	uint32_t  reserved;
	uint64_t  sector;
};

/* Block request types */
#define VIRTIO_BLK_T_IN			0
#define VIRTIO_BLK_T_OUT		1
#define VIRTIO_BLK_T_SCSI_CMD		2
#define VIRTIO_BLK_T_SCSI_CMD_OUT	3
#define VIRTIO_BLK_T_FLUSH		4
#define VIRTIO_BLK_T_FLUSH_OUT		5
#define VIRTIO_BLK_T_BARRIER		0x80000000

/* VIRTIO_BLK Feature bits */
#define VIRTIO_BLK_F_BLK_SIZE       (1 << 6)

extern int virtioblk_init(struct virtio_device *dev);
extern void virtioblk_shutdown(struct virtio_device *dev);
extern int virtioblk_transfer(struct virtio_device *dev, char *buf,
                              uint64_t blocknum, long cnt, unsigned int type);

#endif  /* _VIRTIO_BLK_H */
