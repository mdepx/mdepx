/******************************************************************************
 * Copyright (c) 2020 Hesham Almatary
 * See LICENSE_CHERI for license details.
 *****************************************************************************/

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <cpu.h>
#include <cache.h>
#include <byteorder.h>
#include "virtio.h"
#include "helpers.h"
#include "virtio-internal.h"

#ifdef VIRTIO_USE_MMIO
#include "virtio_mmio.h"
#endif

#ifdef __CHERI_PURE_CAPABILITY__
#include <cheri/cheri-utility.h>
#endif

/* PCI virtio header offsets */
#define VIRTIOHDR_DEVICE_FEATURES	0
#define VIRTIOHDR_GUEST_FEATURES	4
#define VIRTIOHDR_QUEUE_ADDRESS 	8
#define VIRTIOHDR_QUEUE_SIZE		12
#define VIRTIOHDR_QUEUE_SELECT		14
#define VIRTIOHDR_QUEUE_NOTIFY		16
#define VIRTIOHDR_DEVICE_STATUS 	18
#define VIRTIOHDR_ISR_STATUS		19
#define VIRTIOHDR_DEVICE_CONFIG 	20

/* PCI defines */
#define PCI_BASE_ADDR_SPACE_IO	0x01
#define PCI_BASE_ADDR_SPACE_64BIT 0x04
#define PCI_BASE_ADDR_MEM_MASK	(~0x0fUL)
#define PCI_BASE_ADDR_IO_MASK	(~0x03UL)

#define PCI_BASE_ADDR_REG_0	0x10
#define PCI_CONFIG_CAP_REG	0x34

#define PCI_CAP_ID_VNDR		0x9

/* Common configuration */
#define VIRTIO_PCI_CAP_COMMON_CFG       1
/* Notifications */
#define VIRTIO_PCI_CAP_NOTIFY_CFG       2
/* ISR access */
#define VIRTIO_PCI_CAP_ISR_CFG          3
/* Device specific configuration */
#define VIRTIO_PCI_CAP_DEVICE_CFG       4
/* PCI configuration access */
#define VIRTIO_PCI_CAP_PCI_CFG          5

#define VIRTIO_PCI_CAP_VNDR     0	  /* Generic PCI field: PCI_CAP_ID_VNDR */
#define VIRTIO_PCI_CAP_NEXT     1	  /* Generic PCI field: next ptr. */
#define VIRTIO_PCI_CAP_LEN      2	  /* Generic PCI field: capability length */
#define VIRTIO_PCI_CAP_CFG_TYPE 3	  /* Identifies the structure. */
#define VIRTIO_PCI_CAP_BAR      4	  /* Where to find it. */
#define VIRTIO_PCI_CAP_OFFSET   8	  /* Offset within bar. */
#define VIRTIO_PCI_CAP_LENGTH  12	  /* Length of the structure, in bytes. */

struct virtio_dev_common {
	le32 dev_features_sel;
	le32 dev_features;
	le32 drv_features_sel;
	le32 drv_features;
	le16 msix_config;
	le16 num_queues;
	uint8_t dev_status;
	uint8_t cfg_generation;

	le16 q_select;
	le16 q_size;
	le16 q_msix_vec;
	le16 q_enable;
	le16 q_notify_off;
	le64 q_desc;
	le64 q_avail;
	le64 q_used;
} __attribute__ ((packed));

#ifdef VIRTIO_USE_PCI
/* virtio 1.0 Spec: 4.1.3 PCI Device Layout
 *
 * Fields of different sizes are present in the device configuration regions.
 * All 64-bit, 32-bit and 16-bit fields are little-endian. 64-bit fields are to
 * be treated as two 32-bit fields, with low 32 bit part followed by the high 32
 * bit part.
 */
static void virtio_pci_write64(void *addr, uint64_t val)
{
	uint32_t hi = (val >> 32) & 0xFFFFFFFF;
	uint32_t lo = val & 0xFFFFFFFF;

	ci_write_32(addr, cpu_to_le32(lo));
	ci_write_32(addr + 4, cpu_to_le32(hi));
}

static void virtio_cap_set_base_addr(struct virtio_cap *cap, uint32_t offset)
{
	uint64_t addr;

	addr = SLOF_pci_config_read32(PCI_BASE_ADDR_REG_0 + 4 * cap->bar);
	if (addr & PCI_BASE_ADDR_SPACE_IO) {
		addr = addr & PCI_BASE_ADDR_IO_MASK;
		cap->is_io = 1;
	} else {
		if (addr & PCI_BASE_ADDR_SPACE_64BIT)
			addr |= SLOF_pci_config_read32(PCI_BASE_ADDR_REG_0 + 4 * (cap->bar + 1)) << 32;
		addr = addr & PCI_BASE_ADDR_MEM_MASK;
		cap->is_io = 0;
	}
	addr = (uint64_t)SLOF_translate_my_address((void *)addr);
	cap->addr = (void *)addr + offset;
}

static void virtio_process_cap(struct virtio_device *dev, uint8_t cap_ptr)
{
	struct virtio_cap *cap;
	uint8_t cfg_type, bar;
	uint32_t offset;

	cfg_type = SLOF_pci_config_read8(cap_ptr + VIRTIO_PCI_CAP_CFG_TYPE);
	bar = SLOF_pci_config_read8(cap_ptr + VIRTIO_PCI_CAP_BAR);
	offset = SLOF_pci_config_read32(cap_ptr + VIRTIO_PCI_CAP_OFFSET);

	switch(cfg_type) {
	case VIRTIO_PCI_CAP_COMMON_CFG:
		cap = &dev->common;
		break;
	case VIRTIO_PCI_CAP_NOTIFY_CFG:
		cap = &dev->notify;
		dev->notify_off_mul = SLOF_pci_config_read32(cap_ptr + sizeof(struct virtio_cap));
		break;
	case VIRTIO_PCI_CAP_ISR_CFG:
		cap = &dev->isr;
		break;
	case VIRTIO_PCI_CAP_DEVICE_CFG:
		cap = &dev->device;
		break;
	default:
		return;
	}

	cap->bar = bar;
	virtio_cap_set_base_addr(cap, offset);
	cap->cap_id = cfg_type;
}
#endif

#ifdef VIRTIO_USE_MMIO
static void virtio_mmio_print_configs(uint32_t* device_base)
{
	printf("MagicValue:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_MAGIC_VALUE));
	printf("Version:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_VERSION));
	printf("DeviceID:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_DEVICE_ID));
	printf("VendorID:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_VENDOR_ID));
	printf("DeviceFeatures0:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_HOST_FEATURES));
	printf("DeviceFeaturesSel0:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_HOST_FEATURES_SEL));
	virtio_mmio_write32(device_base, VIRTIO_MMIO_HOST_FEATURES_SEL, 1);
	sync();
	printf("DeviceFeatures1:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_HOST_FEATURES));
	printf("DriverFeatures:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_GUEST_FEATURES));
	printf("DriverFeaturesSel:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_GUEST_FEATURES_SEL));
	printf("PageSize:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_GUEST_PAGE_SIZE));
	virtio_mmio_write32(device_base, VIRTIO_MMIO_QUEUE_SEL, 0);
	sync();
	printf("QueueSel:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_SEL));
	printf("QueueNumMax:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_NUM_MAX));
	printf("QueueNum:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_NUM));
	virtio_mmio_write32(device_base, VIRTIO_MMIO_QUEUE_SEL, 1);
	sync();
	printf("QueueSel:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_SEL));
	printf("QueueNumMax1:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_NUM_MAX));
	printf("QueueNum1:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_NUM));
	printf("QueueAlign:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_ALIGN));
	printf("QueuePFN:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_PFN));
	printf("QueueReady:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_READY));
	printf("QueueNotify:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_NOTIFY));
	printf("InterruptStatus:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_INTERRUPT_STATUS));
	printf("InterruptACK:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_INTERRUPT_ACK));
	printf("Status:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_STATUS));
	printf("QueueDescLow:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_DESC_LOW));
	printf("QueueDescHigh:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_DESC_HIGH));
	printf("QueueDriverLow:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_AVAIL_LOW));
	printf("QueueDriverHigh:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_AVAIL_HIGH));
	printf("QueueDeviceLow:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_USED_LOW));
	printf("QueueDeviceHigh:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_QUEUE_USED_HIGH));
	printf("ConfigGeneration:\t 0x%x\n", virtio_mmio_read32(device_base, VIRTIO_MMIO_CONFIG_GENERATION));
	printf("Config:\t 0x%x\n", virtio_mmio_read32(device_base,VIRTIO_MMIO_CONFIG));
}
#endif

/**
 * Reads the virtio device capabilities, gets called from SLOF routines The
 * function determines legacy or modern device and sets up driver registers
 */
struct virtio_device *virtio_setup_vd(void *device_base)
{
#ifdef VIRTIO_USE_PCI
	uint8_t cap_ptr, cap_vndr;
	struct virtio_device *dev;

	dev = SLOF_alloc_mem(sizeof(struct virtio_device));
	if (!dev) {
		printf("Failed to allocate memory");
		return NULL;
	}

	cap_ptr = SLOF_pci_config_read8(PCI_CONFIG_CAP_REG);
	while (cap_ptr != 0) {
		cap_vndr = SLOF_pci_config_read8(cap_ptr + VIRTIO_PCI_CAP_VNDR);
		if (cap_vndr == PCI_CAP_ID_VNDR)
			virtio_process_cap(dev, cap_ptr);
		cap_ptr = SLOF_pci_config_read8(cap_ptr+VIRTIO_PCI_CAP_NEXT);
	}

	if (dev->common.cap_id && dev->notify.cap_id &&
	    dev->isr.cap_id && dev->device.cap_id) {
		dev->features = VIRTIO_F_VERSION_1;
	} else {
		dev->features = 0;
		dev->legacy.cap_id = 0;
		dev->legacy.bar = 0;
		virtio_cap_set_base_addr(&dev->legacy, 0);
	}
	return dev;
#elif VIRTIO_USE_MMIO
	struct virtio_device *dev;
	volatile uint64_t features = 0;

	dev = SLOF_alloc_mem(sizeof(struct virtio_device));
	if (!dev) {
		printf("Failed to allocate memory");
		return NULL;
	}

	// Read the full 64-bit device features field
	virtio_mmio_write32(device_base, VIRTIO_MMIO_HOST_FEATURES_SEL, 0);
	features = virtio_mmio_read32(device_base, VIRTIO_MMIO_HOST_FEATURES);
		virtio_mmio_write32(device_base, VIRTIO_MMIO_HOST_FEATURES_SEL, 1);
	features |= ((uint64_t) virtio_mmio_read32(device_base, VIRTIO_MMIO_HOST_FEATURES) << 32);
	if (features & VIRTIO_F_VERSION_1) {
		dev->features = VIRTIO_F_VERSION_1;
		dev->mmio_base = device_base;
	} else {
		dev->features = 0;
		dev->mmio_base = device_base;
		virtio_mmio_write32(device_base, VIRTIO_MMIO_GUEST_PAGE_SIZE, 0x1000);
	}
	sync();

	return dev;
#endif
}

/**
 * Calculate ring size according to queue size number
 */
unsigned long virtio_vring_size(unsigned int qsize)
{
	return VQ_ALIGN(sizeof(struct vring_desc) * qsize +
			sizeof(struct vring_avail) + sizeof(uint16_t) * qsize) +
		VQ_ALIGN(sizeof(struct vring_used) +
			 sizeof(struct vring_used_elem) * qsize);
}


/**
 * Get number of elements in a vring
 * @param   dev  pointer to virtio device information
 * @param   queue virtio queue number
 * @return  number of elements
 */
unsigned int virtio_get_qsize(struct virtio_device *dev, int queue)
{
#ifdef VIRTIO_USE_PCI
	unsigned int size = 0;

	if (dev->features & VIRTIO_F_VERSION_1) {
		void *addr = dev->common.addr + offset_of(struct virtio_dev_common, q_select);
		ci_write_16(addr, cpu_to_le16(queue));
		eieio();
		addr = dev->common.addr + offset_of(struct virtio_dev_common, q_size);
		size = le16_to_cpu(ci_read_16(addr));
	}
	else {
		ci_write_16(dev->legacy.addr+VIRTIOHDR_QUEUE_SELECT,
			    cpu_to_le16(queue));
		eieio();
		size = le16_to_cpu(ci_read_16(dev->legacy.addr+VIRTIOHDR_QUEUE_SIZE));
	}

	return size;
#elif VIRTIO_USE_MMIO

	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_SEL, queue);
	sync();

	if (dev->features & VIRTIO_F_VERSION_1) {
		return virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_QUEUE_NUM);
	} else {
		// FIXME: This is always reading 0 even if it's written with a different
		// value (only on QEMU/Legacy virtio). Use VIRTIO_MMIO_QUEUE_NUM_MAX instead.
		//return virtio_mmio_read31(VIRTIO_MMIO_QUEUE_NUM);
		return virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_QUEUE_NUM_MAX);
	}
#endif
}


/**
 * Get address of descriptor vring
 * @param   dev  pointer to virtio device information
 * @param   queue virtio queue number
 * @return  pointer to the descriptor ring
 */
struct vring_desc *virtio_get_vring_desc(struct virtio_device *dev, int queue)
{
	return dev->vq[queue].desc;
}


/**
 * Get address of "available" vring
 * @param   dev  pointer to virtio device information
 * @param   queue virtio queue number
 * @return  pointer to the "available" ring
 */
struct vring_avail *virtio_get_vring_avail(struct virtio_device *dev, int queue)
{
	return dev->vq[queue].avail;
}


/**
 * Get address of "used" vring
 * @param   dev  pointer to virtio device information
 * @param   queue virtio queue number
 * @return  pointer to the "used" ring
 */
struct vring_used *virtio_get_vring_used(struct virtio_device *dev, int queue)
{
	return dev->vq[queue].used;
}

/**
 * Fill the virtio ring descriptor depending on the legacy mode or virtio 1.0
 */
void virtio_fill_desc(struct vqs *vq, int id, uint64_t features,
                      uint64_t addr, uint32_t len,
                      uint16_t flags, uint16_t next)
{
	struct vring_desc *desc;

	id %= vq->size;
	desc = &vq->desc[id];
	next %= vq->size;

	if (features & VIRTIO_F_VERSION_1) {
		if (features & VIRTIO_F_IOMMU_PLATFORM) {
			void *gpa = (void *) addr;

			if (!vq->desc_gpas) {
				fprintf(stderr, "IOMMU setup has not been done!\n");
				return;
			}

			addr = SLOF_dma_map_in(gpa, len, 0);
			vq->desc_gpas[id] = gpa;
		}
		desc->addr = cpu_to_le64(addr);
		desc->len = cpu_to_le32(len);
		desc->flags = cpu_to_le16(flags);
		desc->next = cpu_to_le16(next);
	} else {
		desc->addr = addr;
		desc->len = len;
		desc->flags = flags;
		desc->next = next;
	}
}

void virtio_free_desc(struct vqs *vq, int id, uint64_t features)
{
	struct vring_desc *desc;

	id %= vq->size;
	desc = &vq->desc[id];

	if (!(features & VIRTIO_F_VERSION_1) ||
	    !(features & VIRTIO_F_IOMMU_PLATFORM))
		return;

	if (!vq->desc_gpas[id])
		return;

	SLOF_dma_map_out(le64_to_cpu(desc->addr), 0, le32_to_cpu(desc->len));
	vq->desc_gpas[id] = NULL;
}

size_t virtio_desc_addr(struct virtio_device *vdev, int queue, int id)
{
	struct vqs *vq = &vdev->vq[queue];

	if (vq->desc_gpas)
		return (size_t) vq->desc_gpas[id];

	return (size_t) virtio_modern64_to_cpu(vdev, vq->desc[id].addr);
}

/**
 * Reset virtio device
 */
void virtio_reset_device(struct virtio_device *dev)
{
	virtio_set_status(dev, 0);
}


void virtio_queue_ready(struct virtio_device *dev, int queue)
{
#if VIRTIO_USE_MMIO
	if (dev->features & VIRTIO_F_VERSION_1) {
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_SEL, queue);
		sync();
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_READY, 1);
	}
#endif

}
/**
 * Notify hypervisor about queue update
 */
void virtio_queue_notify(struct virtio_device *dev, int queue)
{
#ifdef VIRTIO_USE_PCI
	if (dev->features & VIRTIO_F_VERSION_1) {
		void *q_sel = dev->common.addr + offset_of(struct virtio_dev_common, q_select);
		void *q_ntfy = dev->common.addr + offset_of(struct virtio_dev_common, q_notify_off);
		void *addr;
		uint16_t q_notify_off;

		ci_write_16(q_sel, cpu_to_le16(queue));
		eieio();
		q_notify_off = le16_to_cpu(ci_read_16(q_ntfy));
		addr = dev->notify.addr + q_notify_off * dev->notify_off_mul;
		ci_write_16(addr, cpu_to_le16(queue));
	} else {
		ci_write_16(dev->legacy.addr+VIRTIOHDR_QUEUE_NOTIFY, cpu_to_le16(queue));
	}
#elif VIRTIO_USE_MMIO
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_NOTIFY, queue);
#endif
}

/**
 * Set queue address
 */
static void virtio_set_qaddr(struct virtio_device *dev, int queue, uint64_t qaddr)
{
#ifdef VIRTIO_USE_PCI
	if (dev->features & VIRTIO_F_VERSION_1) {
		uint64_t q_desc = qaddr;
		uint64_t q_avail;
		uint64_t q_used;
		uint32_t q_size = virtio_get_qsize(dev, queue);

		if (dev->features & VIRTIO_F_IOMMU_PLATFORM) {
			unsigned long cb;

			cb = q_size * sizeof(struct vring_desc);
			cb += sizeof(struct vring_avail) +
			      sizeof(uint16_t) * q_size;
			cb = VQ_ALIGN(cb);
			cb += sizeof(struct vring_used) +
			      sizeof(struct vring_used_elem) * q_size;
			cb = VQ_ALIGN(cb);
			q_desc = SLOF_dma_map_in((void *)q_desc, cb, 0);

			dev->vq[queue].bus_desc = q_desc;
		}

		virtio_pci_write64(dev->common.addr + offset_of(struct virtio_dev_common, q_desc), q_desc);
		q_avail = q_desc + q_size * sizeof(struct vring_desc);
		virtio_pci_write64(dev->common.addr + offset_of(struct virtio_dev_common, q_avail), q_avail);
		q_used = VQ_ALIGN(q_avail + sizeof(struct vring_avail) + sizeof(uint16_t) * q_size);
		virtio_pci_write64(dev->common.addr + offset_of(struct virtio_dev_common, q_used), q_used);
		ci_write_16(dev->common.addr + offset_of(struct virtio_dev_common, q_enable), cpu_to_le16(1));
	} else {
		uint32_t val = qaddr;
		val = val >> 12;
		ci_write_16(dev->legacy.addr+VIRTIOHDR_QUEUE_SELECT,
			    cpu_to_le16(queue));
		eieio();
		ci_write_32(dev->legacy.addr+VIRTIOHDR_QUEUE_ADDRESS,
			    cpu_to_le32(val));
	}
#elif VIRTIO_USE_MMIO
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_SEL, queue);
	sync();

	if (dev->features & VIRTIO_F_VERSION_1) {
		uint64_t q_desc = qaddr;
		uint64_t q_avail;
		uint64_t q_used;
		uint32_t q_size = virtio_get_qsize(dev, queue);

		q_avail = q_desc + q_size * sizeof(struct vring_desc);
		q_used = VQ_ALIGN(q_avail + sizeof(struct vring_avail) + sizeof(uint16_t) * q_size);

		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_DESC_LOW, (qaddr & UINT32_MAX));
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_DESC_HIGH, (((uint64_t) qaddr >> 32) & UINT32_MAX));

		// Avail
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_AVAIL_LOW, (q_avail & UINT32_MAX));
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_AVAIL_HIGH, (((uint64_t) q_avail >> 32) & UINT32_MAX));

		// Used
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_USED_LOW, (q_used & UINT32_MAX));
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_USED_HIGH, (((uint64_t) q_used >> 32) & UINT32_MAX));
	} else {
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_PFN, qaddr >> 12);
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_NUM, 1024);
		virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_QUEUE_ALIGN, 0x1000);
	}
#endif
}

struct vqs *virtio_queue_init_vq(struct virtio_device *dev, unsigned int id)
{
	struct vqs *vq;

	if (id >= sizeof(dev->vq)/sizeof(dev->vq[0])) {
		printf("Queue index is too big!\n");
		return NULL;
	}
	vq = &dev->vq[id];

	memset(vq, 0, sizeof(*vq));

	vq->size = virtio_get_qsize(dev, id);
	vq->desc = SLOF_alloc_mem_aligned(virtio_vring_size(vq->size), 4096);
	if (!vq->desc) {
		printf("memory allocation failed!\n");
		return NULL;
	}

	vq->avail = (void *) ((size_t) vq->desc + vq->size * sizeof(struct vring_desc));

#ifdef __CHERI_PURE_CAPABILITY__
	/* Avail ring is  written by the driver */
	vq->avail = cheri_derive_data_cap(vq->desc,
									  (ptraddr_t) vq->avail,
									  sizeof(struct vring_avail) + sizeof(uint16_t) * vq->size,
									  __CHERI_CAP_PERMISSION_PERMIT_LOAD__ |
									  __CHERI_CAP_PERMISSION_PERMIT_STORE__);
#endif

	vq->used = (void *) ((size_t) VQ_ALIGN((size_t) vq->avail +
		sizeof(struct vring_avail) +
		sizeof(uint16_t) * vq->size));

#ifdef __CHERI_PURE_CAPABILITY__
	/* Used ring is only written by the device, and read by the driver */
	vq->used = cheri_derive_data_cap(vq->desc,
									 (ptraddr_t) vq->used,
									 sizeof(struct vring_used) + sizeof(struct vring_used_elem) * vq->size,
									 __CHERI_CAP_PERMISSION_PERMIT_LOAD__);
#endif

	memset(vq->desc, 0, virtio_vring_size(vq->size));
	virtio_set_qaddr(dev, id, (uint64_t)vq->desc);

	vq->avail->flags = virtio_cpu_to_modern16(dev, VRING_AVAIL_F_NO_INTERRUPT);
	vq->avail->idx = 0;
	if (dev->features & VIRTIO_F_IOMMU_PLATFORM)
		vq->desc_gpas = SLOF_alloc_mem_aligned(
			vq->size * sizeof(vq->desc_gpas[0]), 4096);

	return vq;
}

void virtio_queue_term_vq(struct virtio_device *dev, struct vqs *vq, unsigned int id)
{
	if (vq->desc_gpas) {
		uint32_t i;

		for (i = 0; i < vq->size; ++i)
			virtio_free_desc(vq, i, dev->features);

		SLOF_free_mem(vq->desc_gpas,
			vq->size * sizeof(vq->desc_gpas[0]));
	}
	if (vq->desc) {
		if (dev->features & VIRTIO_F_IOMMU_PLATFORM) {
			unsigned long cb;
			uint32_t q_size = virtio_get_qsize(dev, id);

			cb = q_size * sizeof(struct vring_desc);
			cb += sizeof(struct vring_avail) +
			      sizeof(uint16_t) * q_size;
			cb = VQ_ALIGN(cb);
			cb += sizeof(struct vring_used) +
			      sizeof(struct vring_used_elem) * q_size;
			cb = VQ_ALIGN(cb);

			SLOF_dma_map_out(vq->bus_desc, 0, cb);
		}

		SLOF_free_mem(vq->desc, virtio_vring_size(vq->size));
	}
	memset(vq, 0, sizeof(*vq));
}

/**
 * Set device status bits
 */
void virtio_set_status(struct virtio_device *dev, int status)
{
#ifdef VIRTIO_USE_PCI
	if (dev->features & VIRTIO_F_VERSION_1) {
		ci_write_8(dev->common.addr +
			   offset_of(struct virtio_dev_common, dev_status), status);
	} else {
		ci_write_8(dev->legacy.addr+VIRTIOHDR_DEVICE_STATUS, status);
	}
#elif VIRTIO_USE_MMIO
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_STATUS, status);
#endif
}

/**
 * Get device status bits
 */
void virtio_get_status(struct virtio_device *dev, int *status)
{
#ifdef VIRTIO_USE_PCI
	if (dev->features & VIRTIO_F_VERSION_1) {
		*status = ci_read_8(dev->common.addr +
				    offset_of(struct virtio_dev_common, dev_status));
	} else {
		 *status = ci_read_8(dev->legacy.addr+VIRTIOHDR_DEVICE_STATUS);
	}
#elif VIRTIO_USE_MMIO
	*status = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_STATUS);
#endif
}

/**
 * Get device interrupt status bits
 */
void virtio_get_interrupt_status(struct virtio_device *dev, uint32_t *status)
{
#if VIRTIO_USE_MMIO
	*status = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_INTERRUPT_STATUS);
#endif
}

/**
 * Ack device interrupt
 */
void virtio_interrupt_ack(struct virtio_device *dev, uint32_t ack)
{
#if VIRTIO_USE_MMIO
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_INTERRUPT_ACK, ack);
#endif
}
/**
 * Set guest feature bits
 */
void virtio_set_guest_features(struct virtio_device *dev, uint64_t features)

{
#ifdef VIRTIO_USE_PCI
	if (dev->features & VIRTIO_F_VERSION_1) {
		uint32_t f1 = (features >> 32) & 0xFFFFFFFF;
		uint32_t f0 = features & 0xFFFFFFFF;
		void *addr = dev->common.addr;

		ci_write_32(addr + offset_of(struct virtio_dev_common, drv_features_sel),
			    cpu_to_le32(1));
		ci_write_32(addr + offset_of(struct virtio_dev_common, drv_features),
			    cpu_to_le32(f1));

		ci_write_32(addr + offset_of(struct virtio_dev_common, drv_features_sel),
			    cpu_to_le32(0));
		ci_write_32(addr + offset_of(struct virtio_dev_common, drv_features),
			    cpu_to_le32(f0));
	} else {
		ci_write_32(dev->legacy.addr+VIRTIOHDR_GUEST_FEATURES, cpu_to_le32(features));
	}
#elif VIRTIO_USE_MMIO
	uint32_t f1 = (features >> 32) & 0xFFFFFFFF;
	uint32_t f0 = features & 0xFFFFFFFF;

	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_GUEST_FEATURES_SEL, 0);
	sync();
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_GUEST_FEATURES, f0);

	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_GUEST_FEATURES_SEL, 1);
	sync();
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_GUEST_FEATURES, f1);
#endif
}

/**
 * Get host feature bits
 */
uint64_t virtio_get_host_features(struct virtio_device *dev)

{
#ifdef VIRTIO_USE_PCI
	uint64_t features = 0;
	if (dev->features & VIRTIO_F_VERSION_1) {
		uint32_t f0 = 0, f1 = 0;
		void *addr = dev->common.addr;

		ci_write_32(addr + offset_of(struct virtio_dev_common, dev_features_sel),
			    cpu_to_le32(1));
		f1 = ci_read_32(addr +
				offset_of(struct virtio_dev_common, dev_features));
		ci_write_32(addr + offset_of(struct virtio_dev_common, dev_features_sel),
			    cpu_to_le32(0));
		f0 = ci_read_32(addr +
				offset_of(struct virtio_dev_common, dev_features));

		features = ((uint64_t)le32_to_cpu(f1) << 32) | le32_to_cpu(f0);
	} else {
		features = le32_to_cpu(ci_read_32(dev->legacy.addr+VIRTIOHDR_DEVICE_FEATURES));
	}
	return features;
#elif VIRTIO_USE_MMIO
	// Read the full 64-bit device features field
	uint64_t features = 0;
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_HOST_FEATURES_SEL, 0);
	sync();
	features = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_HOST_FEATURES);
	virtio_mmio_write32(dev->mmio_base, VIRTIO_MMIO_HOST_FEATURES_SEL, 1);
	sync();
	features |= ((uint64_t) virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_HOST_FEATURES) << 32);
	return features;
#endif
}

int virtio_negotiate_guest_features(struct virtio_device *dev, uint64_t features)
{
	uint64_t host_features = 0;
	int status;

	/* Negotiate features */
	host_features = virtio_get_host_features(dev);
	if (!(host_features & VIRTIO_F_VERSION_1)) {
		fprintf(stderr, "Device does not support virtio 1.0 %llx\n", host_features);
		return -1;
	}

	if (host_features & VIRTIO_F_IOMMU_PLATFORM)
		features |= VIRTIO_F_IOMMU_PLATFORM;

	virtio_set_guest_features(dev,  features);
	host_features = virtio_get_host_features(dev);
	if ((host_features & features) != features) {
		fprintf(stderr, "Features error %llx\n", features);
		return -1;
	}

	virtio_get_status(dev, &status);
	status |= VIRTIO_STAT_FEATURES_OK;
	virtio_set_status(dev, status);

	/* Read back to verify the FEATURES_OK bit */
	virtio_get_status(dev, &status);
	if ((status & VIRTIO_STAT_FEATURES_OK) != VIRTIO_STAT_FEATURES_OK)
		return -1;

	dev->features = features;

	return 0;
}

/**
 * Get additional config values
 */
uint64_t virtio_get_config(struct virtio_device *dev, int offset, int size)
{
#ifdef VIRTIO_USE_PCI
	uint64_t val = ~0ULL;
	uint32_t hi, lo;
	void *confbase;

	if (dev->features & VIRTIO_F_VERSION_1)
		confbase = dev->device.addr;
	else
		confbase = dev->legacy.addr+VIRTIOHDR_DEVICE_CONFIG;

	switch (size) {
	case 1:
		val = ci_read_8(confbase+offset);
		break;
	case 2:
		val = ci_read_16(confbase+offset);
		if (dev->features & VIRTIO_F_VERSION_1)
			val = le16_to_cpu(val);
		break;
	case 4:
		val = ci_read_32(confbase+offset);
		if (dev->features & VIRTIO_F_VERSION_1)
			val = le32_to_cpu(val);
		break;
	case 8:
		/* We don't support 8 bytes PIO accesses
		 * in qemu and this is all PIO
		 */
		lo = ci_read_32(confbase+offset);
		hi = ci_read_32(confbase+offset+4);
		if (dev->features & VIRTIO_F_VERSION_1)
			val = (uint64_t)le32_to_cpu(hi) << 32 | le32_to_cpu(lo);
		else
			val = (uint64_t)hi << 32 | lo;
		break;
	}

	return val;
#elif VIRTIO_USE_MMIO
	uint64_t val = ~0ULL;
	uint32_t hi, lo;

	switch (size) {
	case 1:
		val = virtio_mmio_read8(dev->mmio_base, VIRTIO_MMIO_CONFIG + offset);
		break;
	case 2:
		val = virtio_mmio_read16(dev->mmio_base, VIRTIO_MMIO_CONFIG + offset);
		break;
	case 4:
		val = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_CONFIG + offset);
		break;
	case 8:
		/* We don't support 8 bytes PIO accesses
		 * in qemu and this is all PIO
		 */
		lo = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_CONFIG + offset);
		hi = virtio_mmio_read32(dev->mmio_base, VIRTIO_MMIO_CONFIG + offset + 4);
		val = (uint64_t)hi << 32 | lo;
	}

	return val;
#endif
}

/**
 * Get config blob
 */
int __virtio_read_config(struct virtio_device *dev, void *dst,
			 int offset, int len)
{
#ifdef VIRTIO_USE_PCI
	void *confbase;
	unsigned char *buf = dst;
	int i;

	if (dev->features & VIRTIO_F_VERSION_1)
		confbase = dev->device.addr;
	else
		confbase = dev->legacy.addr+VIRTIOHDR_DEVICE_CONFIG;

	for (i = 0; i < len; i++)
		buf[i] = ci_read_8(confbase + offset + i);

	return len;
#elif VIRTIO_USE_MMIO
    return 0;
#endif
}
