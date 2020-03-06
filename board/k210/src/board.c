/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#include <sys/cdefs.h>
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/sem.h>
#include <sys/list.h>
#include <sys/smp.h>

#include <machine/pcpu.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <riscv/sifive/e300g_clint.h>
#include <riscv/kendryte/k210.h>

#include "board.h"

#define	CPU_FREQ		400000000
#define	DEFAULT_BAUDRATE	115200
#define	PIN_UARTHS_RX		4
#define	PIN_UARTHS_TX		5

static struct k210_fpioa_softc fpioa_sc;
static struct k210_sysctl_softc sysctl_sc;
static struct k210_uarths_softc uarths_sc;
static struct k210_gpio_softc gpio_sc;
static struct clint_softc clint_sc;

extern uint8_t __riscv_boot_ap[2];
extern uint32_t _sbss;
extern uint32_t _ebss;

#ifdef MDX_SCHED_SMP
uint8_t mp_release __section(".data") = 0;
#endif

static void
uart_putchar(int c, void *arg)
{
	struct uart_softc *sc;

	sc = arg;

	if (sc == NULL)
		return;

	if (c == '\n')
		k210_uarths_putc(&uarths_sc, '\r');

	k210_uarths_putc(&uarths_sc, c);
}

static void
pins_configure(void)
{
	struct fpioa_io_config cfg;

	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_UARTHS_RX;
	cfg.ie_en = 1;
	cfg.st = 1;
	k210_fpioa_set_config(&fpioa_sc, PIN_UARTHS_RX, &cfg);

	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_UARTHS_TX;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, PIN_UARTHS_TX, &cfg);
}

void
board_init(void)
{

	k210_sysctl_init(&sysctl_sc, BASE_SYSCTL);
	k210_fpioa_init(&fpioa_sc, BASE_FPIOA);
	k210_gpio_init(&gpio_sc, BASE_GPIO);
	e300g_clint_init(&clint_sc, BASE_CLINT, 8000000);

	malloc_init();
	malloc_add_region(0x40000000, 6 * 1024 * 1024);

	pins_configure();

	k210_uarths_init(&uarths_sc, BASE_UARTHS, CPU_FREQ, DEFAULT_BAUDRATE);
	mdx_console_register(uart_putchar, (void *)&uarths_sc);

#ifdef MDX_SCHED_SMP
	printf("Releasing CPU 1...\n");
	__riscv_boot_ap[1] = 1;
	clint_set_sip(1);
#endif
}
