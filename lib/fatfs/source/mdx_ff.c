/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ff.h"
#include "diskio.h"

#define	DEV_RAM		0
#define	DEV_MMC		1
#define	DEV_USB		2

DSTATUS
disk_status(BYTE pdrv)
{

	/* TODO */

	switch (pdrv) {
	case DEV_RAM:
	case DEV_MMC:
	case DEV_USB:
		break;
	}

	return (STA_NOINIT);
}

DSTATUS
disk_initialize(BYTE pdrv)
{

	/* TODO */

	switch (pdrv) {
	case DEV_RAM:
	case DEV_MMC:
	case DEV_USB:
		break;
	}

	return (STA_NOINIT);
}

DRESULT
disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{

	/* TODO */

	switch (pdrv) {
	case DEV_RAM:
	case DEV_MMC:
	case DEV_USB:
		break;
	}

	return (RES_PARERR);
}

#if FF_FS_READONLY == 0
DRESULT
disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{

	/* TODO */

	switch (pdrv) {
	case DEV_RAM:
	case DEV_MMC:
	case DEV_USB:
		break;
	}

	return (RES_PARERR);
}
#endif

DRESULT
disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{

	/* TODO */

	switch (pdrv) {
	case DEV_RAM:
	case DEV_MMC:
	case DEV_USB:
		break;
	}

	return (RES_PARERR);
}

DWORD
get_fattime(void)
{

	/* TODO */

	return (0);
}
