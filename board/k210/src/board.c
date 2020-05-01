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
#include <sys/list.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/pcpu.h>
#include <sys/sem.h>
#include <sys/smp.h>
#include <sys/spinlock.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <dev/uart/uart_16550.h>

#include <riscv/sifive/e300g_clint.h>
#include <riscv/kendryte/k210.h>

#include "board.h"

#define	CPU_FREQ		390000000
#define	DEFAULT_BAUDRATE	115200
#define	PIN_UARTHS_RX		4
#define	PIN_UARTHS_TX		5
#define	PIN_GPIO_LED0		4
#define	PIN_GPIO_LED1		5

static struct k210_fpioa_softc fpioa_sc;
static struct k210_sysctl_softc sysctl_sc;
static struct k210_uarths_softc uarths_sc;
static struct clint_softc clint_sc;
struct k210_gpio_softc gpio_sc;
struct k210_gpiohs_softc gpiohs_sc;
struct k210_i2c_softc i2c_sc;
struct uart_16550_softc uart_sc;

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

void
udelay(uint32_t usec)
{

	mdx_usleep(usec);
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

	/* LED 0 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIO4;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 12, &cfg);

	/* LED 1 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIO5;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 13, &cfg);

	/* Set some random pins: not in use. */

	/* GPIOHS20 -> PIN 36 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIOHS20;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 36, &cfg);

	/* GPIOHS21 -> PIN 37 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIOHS21;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 37, &cfg);

	/* GPIOHS22 -> PIN 38 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIOHS22;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 38, &cfg);

	/* GPIOHS26 -> PIN 42 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIOHS26;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 42, &cfg);

	/* GPIOHS27 -> PIN 43 */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_GPIOHS27;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 43, &cfg);

	/* UART TX */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_UART1_TX;
	cfg.ds = 0xf;
	cfg.oe_en = 1;
	k210_fpioa_set_config(&fpioa_sc, 44, &cfg);

	/* UART RX */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_UART1_RX;
	cfg.ie_en = 1;
	cfg.st = 1;
	k210_fpioa_set_config(&fpioa_sc, 45, &cfg);

	/* I2C */
	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_I2C0_SCLK;
	/*
	 * Bitbang ?
	 * cfg.ch_sel = FPIOA_FUNC_GPIOHS28;
	 */
	cfg.oe_en = 1;
	cfg.pu = 1;
	cfg.sl = 1;
	cfg.ie_en = 1;
	cfg.st = 1;
	k210_fpioa_set_config(&fpioa_sc, 46, &cfg);

	bzero(&cfg, sizeof(struct fpioa_io_config));
	cfg.ch_sel = FPIOA_FUNC_I2C0_SDA;
	/*
	 * Bitbang ?
	 * cfg.ch_sel = FPIOA_FUNC_GPIOHS29;
	 */
	cfg.oe_en = 1;
	cfg.pu = 1;
	cfg.sl = 1;
	cfg.ie_en = 1;
	cfg.st = 1;
	k210_fpioa_set_config(&fpioa_sc, 47, &cfg);
}

void
board_init(void)
{

	k210_sysctl_init(&sysctl_sc, BASE_SYSCTL);
	k210_fpioa_init(&fpioa_sc, BASE_FPIOA);
	pins_configure();
	e300g_clint_init(&clint_sc, BASE_CLINT, 8000000);

	/* GPIOHS */
	k210_gpiohs_init(&gpiohs_sc, BASE_GPIOHS);
	mdx_gpio_bank_register(0, &k210_gpiohs_ops, &gpiohs_sc);

	/* GPIO */
	k210_gpio_init(&gpio_sc, BASE_GPIO);
	mdx_gpio_bank_register(1, &k210_gpio_ops, &gpio_sc);

	/* Enable LEDs. */
	mdx_gpio_configure(1, PIN_GPIO_LED0, MDX_GPIO_OUTPUT);
	mdx_gpio_configure(1, PIN_GPIO_LED1, MDX_GPIO_OUTPUT);
	mdx_gpio_set(1, PIN_GPIO_LED0, 0);
	mdx_gpio_set(1, PIN_GPIO_LED1, 0);

	malloc_init();
	malloc_add_region(0x40000000, 6 * 1024 * 1024);

	k210_uarths_init(&uarths_sc, BASE_UARTHS, CPU_FREQ, DEFAULT_BAUDRATE);
	mdx_console_register(uart_putchar, (void *)&uarths_sc);

	k210_i2c_init(&i2c_sc, BASE_I2C0);
	k210_i2c_configure_master(&i2c_sc, 790000000, 400000);

	uart_16550_init(&uart_sc, BASE_UART1, 2);
	uart_16550_configure(&uart_sc, 200000000, 9600, UART_BITWIDTH_8,
	    UART_STOP_1, UART_PARITY_NONE);

#ifdef MDX_SCHED_SMP
	printf("Releasing CPU 1...\n");
	__riscv_boot_ap[1] = 1;
	clint_set_sip(1);
#endif
}
