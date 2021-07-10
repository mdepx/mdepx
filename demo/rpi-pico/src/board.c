/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/malloc.h>
#include <sys/thread.h>
#include <sys/smp.h>

#include <machine/cpufunc.h>
#include <machine/cpuregs.h>
#include <machine/scs.h>

#include <dev/intc/intc.h>
#include <dev/uart/uart.h>

#include <arm/arm/nvic.h>
#include <arm/raspberrypi/rp2040.h>

static struct rp2040_timer_softc timer_sc;
static struct rp2040_xosc_softc xosc_sc;
static struct rp2040_clocks_softc clocks_sc;
static struct rp2040_resets_softc resets_sc;
static struct rp2040_io_bank0_softc io_bank0_sc;
static struct rp2040_uart_softc uart_sc;
static struct rp2040_uart_softc uart1_sc;
static struct rp2040_pll_softc pll_sys_sc;
static struct rp2040_pll_softc pll_usb_sc;
static struct rp2040_psm_softc psm_sc;
static struct rp2040_sio_softc sio_sc;
static struct rp2040_watchdog_softc watchdog_sc;
static struct arm_nvic_softc nvic_sc;

struct mdx_device dev_nvic = { .sc = &nvic_sc };
struct mdx_device dev_uart = { .sc = &uart_sc };
struct mdx_device dev_uart1 = { .sc = &uart1_sc };

extern uint8_t idle_thread_stack[MDX_CPU_MAX][MDX_THREAD_STACK_SIZE];
extern uint8_t intr_stack[MDX_CPU_MAX][MDX_ARM_INTR_STACK_SIZE];
void core1_startup(void);
void core1_boot(void);

static int cpu_started;

void
send_ipi(int mask, int ipi)
{

	/* Ignore the cpu mask since we have two cpu only on rp2040. */

	rp2040_sio_ipi(&sio_sc, ipi);
}

int
get_coreid(void)
{
	int cpuid;

	cpuid = *(volatile uint32_t *)0xd0000000;
	cpuid &= 1;

	return (cpuid);
}

void
udelay(uint32_t usec)
{
	int i;

	/* TODO: implement me */

	for (i = 0; i < usec * 100; i++)
		;
}

void
usleep(uint32_t usec)
{

	mdx_usleep(usec);
}

void
core1_boot(void)
{
	struct pcpu *pcpup;

	rp2040_sio_fifo_drain(&sio_sc);

	pcpup = &__pcpu[1];
	pcpup->pc_cpuid = 1;
	list_init(&pcpup->pc_avail);
	mdx_thread_init(1);
	pcpup->pc_stack = (uintptr_t)&intr_stack[1] +
	    MDX_ARM_INTR_STACK_SIZE;
	__asm __volatile("msr msp, %0" :: "r"(pcpup->pc_stack));

	mdx_sched_cpu_add(pcpup);
	mdx_sched_cpu_avail(pcpup, true);

	mdx_intc_setup(&dev_nvic, RP2040_TIMER_IRQ_1, rp2040_timer_intr,
	    &timer_sc);
	mdx_intc_enable(&dev_nvic, RP2040_TIMER_IRQ_1);

	mdx_intc_setup(&dev_nvic, RP2040_SIO_IRQ_PROC1, rp2040_sio_intr,
	    &sio_sc);
	mdx_intc_enable(&dev_nvic, RP2040_SIO_IRQ_PROC1);

	cpu_started = 1;

	mdx_sched_enter();

	panic("reached unreachable place");
}

static void __unused
secondary_start(void)
{
	uint32_t core1_boot_msg[5];
	uint32_t *sp;
	int i;

	rp2040_psm_reset_core1(&psm_sc);

	sp = (void *)((uint32_t)&idle_thread_stack[1] + MDX_THREAD_STACK_SIZE);

	core1_boot_msg[0] = 0;
	core1_boot_msg[1] = 1;
	core1_boot_msg[2] = *(volatile uint32_t *)0xe000ed08;
	core1_boot_msg[3] = (uint32_t)sp;
	core1_boot_msg[4] = (uint32_t)core1_startup;

	rp2040_sio_fifo_drain(&sio_sc);

	for (i = 0; i < 5; i++)
		if (!rp2040_sio_fifo_comm(&sio_sc, core1_boot_msg[i])) {
			printf("could not start CPU1\n");
			return;
		}

	rp2040_sio_fifo_drain(&sio_sc);
}

void
board_init(void)
{

	malloc_init();
	malloc_add_region((void *)0x20020000, 0x20000);

	rp2040_watchdog_init(&watchdog_sc, RP2040_WATCHDOG_BASE);
	rp2040_xosc_init(&xosc_sc, RP2040_XOSC_BASE);
	rp2040_sio_init(&sio_sc, RP2040_SIO_BASE);
	rp2040_psm_init(&psm_sc, RP2040_PSM_BASE);

	rp2040_resets_init(&resets_sc, RP2040_RESETS_BASE);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_UART0, 1);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_PLL_SYS, 1);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_PLL_USB, 1);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_PLL_SYS, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_PLL_USB, 0);

	rp2040_clocks_init(&clocks_sc, RP2040_CLOCKS_BASE);

	/* 1500 MHz / 6 / 2 = 125 MHz */
	rp2040_pll_init(&pll_sys_sc,  RP2040_PLL_SYS_BASE,
		1, 1500000000, 6, 2);

	/* 480 MHz / 5 / 2 = 48 MHz */
	rp2040_pll_init(&pll_usb_sc,  RP2040_PLL_USB_BASE,
		1, 480000000, 5, 2);

	rp2040_clocks_setup(&clocks_sc);

	rp2040_resets_reset(&resets_sc, RESETS_RESET_IOBANK0, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_UART0, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_UART1, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_TIMER, 0);

	rp2040_io_bank0_init(&io_bank0_sc, RP2040_IO_BANK0_BASE);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 0,
	    IO_BANK0_GPIO0_CTRL_FUNCSEL_UART0_TX);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 1,
	    IO_BANK0_GPIO1_CTRL_FUNCSEL_UART0_RX);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 4,
	    IO_BANK0_GPIO4_CTRL_FUNCSEL_UART1_TX);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 5,
	    IO_BANK0_GPIO5_CTRL_FUNCSEL_UART1_RX);

	arm_nvic_init(&dev_nvic, BASE_NVIC);
	mdx_intc_setup(&dev_nvic, RP2040_TIMER_IRQ_0, rp2040_timer_intr,
	    &timer_sc);
	mdx_intc_enable(&dev_nvic, RP2040_TIMER_IRQ_0);

	rp2040_uart_init(&dev_uart, RP2040_UART0_BASE);
	rp2040_uart_init(&dev_uart1, RP2040_UART1_BASE);
	mdx_uart_setup(&dev_uart, 115200, UART_DATABITS_8, UART_STOPBITS_1,
	    UART_PARITY_NONE);
	mdx_uart_setup(&dev_uart1, 115200, UART_DATABITS_8, UART_STOPBITS_1,
	    UART_PARITY_NONE);
	mdx_console_register_uart(&dev_uart);

	rp2040_timer_init(&timer_sc, RP2040_TIMER_BASE, 1000000);

	printf("%s: board initialized\n", __func__);

	cpu_started = 0;
	secondary_start();
	while (cpu_started == 0)
		cpu_nullop();

	mdx_intc_setup(&dev_nvic, RP2040_SIO_IRQ_PROC0, rp2040_sio_intr,
	    &sio_sc);
	mdx_intc_enable(&dev_nvic, RP2040_SIO_IRQ_PROC0);
}
