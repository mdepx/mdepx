/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/display/panel.h>
#include <dev/display/dsi.h>
#include <dev/otm8009a/otm8009a.h>
#include <dev/intc/intc.h>

#include <arm/stm/stm32f4.h>
#include <arm/arm/nvic.h>

#include <libfont/libfont.h>

#include "board.h"
#include "gpio.h"

struct stm32f4_usart_softc usart_sc;
struct stm32f4_gpio_softc gpio_sc;
struct stm32f4_fmc_softc fmc_sc;
struct stm32f4_flash_softc flash_sc;
struct stm32f4_pwr_softc pwr_sc;
struct stm32f4_rcc_softc rcc_sc;
struct stm32f4_ltdc_softc ltdc_sc;
struct stm32f4_dsi_softc dsi_sc;
struct stm32f4_timer_softc timer_sc;

struct arm_nvic_softc nvic_sc;
struct mdx_device dev_nvic = { .sc = &nvic_sc };

dsi_device_t dsi_dev;

extern uint32_t _smem;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

static const struct sdram sdram_entry = {
	.sdrtr = 1385,
	.nrfs = 7,
	.bank1 = {
		.sdcr = { 1, 1, 2, 0, 3, 1, 2, 0, 0 },
		.sdtr = { 2, 2, 2, 7, 4, 7, 2 },
		.sdcmr = { 0 },
	},
};

static const struct layer_info layers[1] = {{
	.width = DISPLAY_WIDTH,
	.height = DISPLAY_HEIGHT,
	.hsync = 120,
	.hfp = 120,
	.hbp = 120,
	.vsync = 12,
	.vfp = 12,
	.vbp = 12,
	.bpp = 24,
	.base = FB_BASE,
}};

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

static void
uart_putchar(int c, void *arg)
{
	struct stm32f4_usart_softc *sc;
 
	sc = arg;
 
	if (c == '\n')
		stm32f4_usart_putc(sc, '\r');

	stm32f4_usart_putc(sc, c);
}

static void
sdram_memtest(void)
{
	uint32_t *addr;
	int i;

	addr = (uint32_t *)FB_BASE;

	for (i = 0; i < (1024 * 1024); i++) {
		/* Test */
		*(volatile uint32_t *)(addr + i) = 0xaaaaaaaa;
		if (*(volatile uint32_t *)(addr + i) != 0xaaaaaaaa) {
			printf("sdram test failed %x\n",
			    *(volatile uint32_t *)(addr + i));
				while (1);
		}
	}

	printf("sdram test completed\n");
}

void
board_init(void)
{
	struct stm32f4_dsi_config dconf;
	struct stm32f4_rcc_pll_conf pconf;
	uint32_t reg;

	stm32f4_flash_init(&flash_sc, FLASH_BASE);
	stm32f4_pwr_init(&pwr_sc, PWR_BASE);
	stm32f4_rcc_init(&rcc_sc, RCC_BASE);

	pconf.pllm = 8;
	pconf.plln = 336;
	pconf.pllq = 7;
	pconf.pllp = 0;
	pconf.external = 1;
	pconf.rcc_cfgr = (CFGR_PPRE2_2 | CFGR_PPRE1_4);
	stm32f4_rcc_pll_configure(&rcc_sc, &pconf);
	stm32f4_rcc_pllsai(&rcc_sc, 192, 7, 4);

	reg = (GPIOAEN | GPIOBEN | GPIOCEN);
	reg |= (GPIODEN | GPIOEEN | GPIOFEN);
	reg |= (GPIOGEN | GPIOHEN | GPIOIEN);
	reg |= (GPIOJEN | GPIOKEN);

	stm32f4_flash_setup(&flash_sc);
	stm32f4_rcc_setup(&rcc_sc, reg, 0, FMCEN,
	    (PWREN), (TIM1EN | USART6EN | LTDCEN | DSIEN));

	stm32f4_gpio_init(&gpio_sc, GPIO_BASE);
	gpio_config(&gpio_sc);

	stm32f4_fmc_init(&fmc_sc, FMC_BASE);
	stm32f4_fmc_setup(&fmc_sc, &sdram_entry);

	stm32f4_usart_init(&usart_sc, USART6_BASE, 84000000, 115200);
	mdx_console_register(uart_putchar, (void *)&usart_sc);

	/* (168/4) * 2 = 84MHz / 1 PSC = 84 */
	stm32f4_timer_init(&timer_sc, TIM1_BASE, (84000000 * 2));
	arm_nvic_init(&dev_nvic, NVIC_BASE);

	mdx_intc_setup(&dev_nvic, 27, stm32f4_timer_intr, &timer_sc);
	mdx_intc_enable(&dev_nvic, 27);

	sdram_memtest();

	stm32f4_ltdc_init(&ltdc_sc, LTDC_BASE);
	stm32f4_ltdc_setup(&ltdc_sc, layers, 1);

	/* Reset DSI LCD */
	pin_set(&gpio_sc, PORT_H, 7, 0);
	udelay(10000);
	pin_set(&gpio_sc, PORT_H, 7, 1);

	stm32f4_dsi_init(&dsi_sc, &dsi_dev, DSI_BASE);
	dsi_dev.vchid = 0;

	dconf.ndiv = 125;
	dconf.idf = 2;
	dconf.odf = 1;
	dconf.nlanes = 2;
	dconf.video_mode = 1;
	dconf.hse_val = BOARD_OSC_FREQ;

	stm32f4_dsi_setup(&dsi_sc, &dconf, &layers[0]);

	otm8009a_init(&dsi_dev, OTM_FMT_RGB888,
	    OTM_ORIENTATION_PORTRAIT);
}
