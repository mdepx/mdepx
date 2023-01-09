/*-
 * Based upon the software originally developed by
 *   Raspberry Pi (Trading) Ltd.
 *
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/systm.h>

#include <arm/raspberrypi/rp2040_pio.h>
#include <arm/raspberrypi/rp2040_pio_regs.h>
#include <arm/raspberrypi/pio_instructions.h>

#include <assert.h>

#define	RP2040_PIO_DEBUG
#undef	RP2040_PIO_DEBUG

#ifdef	RP2040_PIO_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static_assert(PIO_INSTRUCTION_COUNT <= 32, "error");

static uint32_t
hw_claim_lock(void)
{

	critical_enter();

	return (0);
}

static void
hw_claim_unlock(uint32_t saved __unused)
{

	critical_exit();
}

static int
_pio_find_offset_for_program(struct rp2040_pio_softc *sc,
    const rp2040_pio_program_t *program)
{
	uint32_t program_mask;
	uint32_t used_mask;

	assert(program->length <= PIO_INSTRUCTION_COUNT);

	used_mask = sc->used_instruction_space;
	program_mask = (1u << program->length) - 1;

	if (program->origin >= 0) {
		if (program->origin > 32 - program->length)
			return -1;
		return used_mask & (program_mask << program->origin) ?
		    -1 : program->origin;
	} else {
		// work down from the top always
		for (int i = 32 - program->length; i >= 0; i--) {
			if (!(used_mask & (program_mask << (uint) i)))
				return i;
		}
		return -1;
	}
}

bool
pio_can_add_program(mdx_device_t dev, const rp2040_pio_program_t *program)
{
	struct rp2040_pio_softc *sc;
	uint32_t save;
	bool rc;

	sc = mdx_device_get_softc(dev);

	save = hw_claim_lock();
	rc = -1 != _pio_find_offset_for_program(sc, program);
	hw_claim_unlock(save);

	return (rc);
}

static bool
_pio_can_add_program_at_offset(struct rp2040_pio_softc *sc,
    const rp2040_pio_program_t *program, uint offset)
{
	uint32_t program_mask;
	uint32_t used_mask;

	valid_params_if(sc, offset < PIO_INSTRUCTION_COUNT);
	valid_params_if(sc, offset + program->length <= PIO_INSTRUCTION_COUNT);

	if (program->origin >= 0 && (uint)program->origin != offset)
		return false;
	used_mask = sc->used_instruction_space;
	program_mask = (1u << program->length) - 1;

	return !(used_mask & (program_mask << offset));
}

static void
_pio_add_program_at_offset(struct rp2040_pio_softc *sc,
    const rp2040_pio_program_t *program, uint offset)
{
	uint32_t program_mask;
	uint32_t reg;
	uint16_t instr;

	if (!_pio_can_add_program_at_offset(sc, program, offset))
		panic("No program space");

	for (uint i = 0; i < program->length; ++i) {
		instr = program->instructions[i];
		reg = pio_instr_bits_jmp != _pio_major_instr_bits (instr) ?
		    instr : instr + offset;
		WR4(sc, RP2040_PIO_INSTR_MEM_OFFSET(offset + i), reg);
	}
	program_mask = (1u << program->length) - 1;
	sc->used_instruction_space |= program_mask << offset;
}

uint
pio_add_program(mdx_device_t dev, const rp2040_pio_program_t *program)
{
	struct rp2040_pio_softc *sc;
	uint32_t save;
	int offset;

	sc = mdx_device_get_softc(dev);

	save = hw_claim_lock();
	offset = _pio_find_offset_for_program(sc, program);
	if (offset < 0)
		panic("No program space");
	_pio_add_program_at_offset(sc, program, (uint)offset);
	hw_claim_unlock(save);

	return (uint)offset;
}

int
rp2040_pio_init(mdx_device_t dev, uint32_t base)
{
	struct rp2040_pio_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	return (0);
}

void
rp2040_sm_config_set_clkdiv_int_frac(struct rp2040_pio_sm_config *c,
    uint16_t div_int, uint8_t div_frac)
{

	c->clkdiv = (((uint32_t)div_frac) << RP2040_PIO_SM_CLKDIV_FRAC_SHIFT) |
	    (((uint32_t)div_int) << RP2040_PIO_SM_CLKDIV_INT_SHIFT);
}

void
rp2040_sm_config_set_wrap(struct rp2040_pio_sm_config *c, uint32_t wrap_target,
    uint32_t wrap)
{

	valid_params_if(PIO, wrap < PIO_INSTRUCTION_COUNT);
	valid_params_if(PIO, wrap_target < PIO_INSTRUCTION_COUNT);

	c->execctrl = (c->execctrl & ~(RP2040_PIO_SM_EXECCTRL_WRAP_TOP_MASK |
	    RP2040_PIO_SM_EXECCTRL_WRAP_BOTTOM_MASK)) |
	    (wrap_target << RP2040_PIO_SM_EXECCTRL_WRAP_BOTTOM_SHIFT) |
	    (wrap << RP2040_PIO_SM_EXECCTRL_WRAP_TOP_SHIFT);
}

void
rp2040_sm_config_set_in_shift(struct rp2040_pio_sm_config *c,
    bool shift_right, bool autopush, uint32_t push_threshold)
{

	valid_params_if(PIO, push_threshold <= 32);

	c->shiftctrl = (c->shiftctrl & ~(RP2040_PIO_SM_SHIFTCTRL_IN_SHIFTDIR |
	    RP2040_PIO_SM_SHIFTCTRL_AUTOPUSH |
	    RP2040_PIO_SM_SHIFTCTRL_PUSH_THRESH_MASK)) |
	    (shift_right ? RP2040_PIO_SM_SHIFTCTRL_IN_SHIFTDIR : 0) |
	    (autopush ? RP2040_PIO_SM_SHIFTCTRL_AUTOPUSH : 0) |
	    ((push_threshold & 0x1fu) <<
		RP2040_PIO_SM_SHIFTCTRL_PUSH_THRESH_SHIFT);
}

void
rp2040_sm_config_set_out_shift(struct rp2040_pio_sm_config *c,
    bool shift_right, bool autopull, uint32_t pull_threshold)
{

	valid_params_if(PIO, pull_threshold <= 32);

	c->shiftctrl = (c->shiftctrl & ~(RP2040_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR |
	    RP2040_PIO_SM_SHIFTCTRL_AUTOPULL |
	    RP2040_PIO_SM_SHIFTCTRL_PULL_THRESH_MASK)) |
	    (shift_right ? RP2040_PIO_SM_SHIFTCTRL_OUT_SHIFTDIR : 0) |
	    (autopull ? RP2040_PIO_SM_SHIFTCTRL_AUTOPULL : 0) |
	    ((pull_threshold & 0x1fu) <<
		RP2040_PIO_SM_SHIFTCTRL_PULL_THRESH_SHIFT);
}

void
rp2040_sm_config_set_out_pins(struct rp2040_pio_sm_config *c, uint32_t out_base,
    uint32_t out_count)
{

	valid_params_if(PIO, out_base < 32);
	valid_params_if(PIO, out_count <= 32);

	c->pinctrl = (c->pinctrl & ~(RP2040_PIO_SM_PINCTRL_OUT_BASE_MASK |
	    RP2040_PIO_SM_PINCTRL_OUT_COUNT_MASK)) |
	    (out_base << RP2040_PIO_SM_PINCTRL_OUT_BASE_SHIFT) |
	    (out_count << RP2040_PIO_SM_PINCTRL_OUT_COUNT_SHIFT);
}

void
rp2040_sm_config_set_in_pins(struct rp2040_pio_sm_config *c, uint32_t in_base)
{

	valid_params_if(PIO, in_base < 32);

	c->pinctrl = (c->pinctrl & ~RP2040_PIO_SM_PINCTRL_IN_BASE_MASK) |
	    (in_base << RP2040_PIO_SM_PINCTRL_IN_BASE_SHIFT);
}

void
rp2040_sm_config_set_set_pins(struct rp2040_pio_sm_config *c,
    uint32_t set_base, uint32_t set_count)
{

	valid_params_if(PIO, set_base < 32);
	valid_params_if(PIO, set_count <= 5);

	c->pinctrl = (c->pinctrl & ~(RP2040_PIO_SM_PINCTRL_SET_BASE_MASK |
	    RP2040_PIO_SM_PINCTRL_SET_COUNT_MASK)) |
	    (set_base << RP2040_PIO_SM_PINCTRL_SET_BASE_SHIFT) |
	    (set_count << RP2040_PIO_SM_PINCTRL_SET_COUNT_SHIFT);
}

void
rp2040_sm_config_set_sideset(struct rp2040_pio_sm_config *c,
    uint32_t bit_count, bool optional, bool pindirs)
{

	valid_params_if(PIO, bit_count <= 5);
	valid_params_if(PIO, !optional || bit_count >= 1);

	c->pinctrl = (c->pinctrl & ~RP2040_PIO_SM_PINCTRL_SIDESET_COUNT_MASK) |
	    (bit_count << RP2040_PIO_SM_PINCTRL_SIDESET_COUNT_SHIFT);

	c->execctrl = (c->execctrl & ~(RP2040_PIO_SM_EXECCTRL_SIDE_EN |
	    RP2040_PIO_SM_EXECCTRL_SIDE_PINDIR)) |
	    (optional ? RP2040_PIO_SM_EXECCTRL_SIDE_EN : 0) |
	    (pindirs ? RP2040_PIO_SM_EXECCTRL_SIDE_PINDIR : 0);
}

void
rp2040_sm_config_set_sideset_pins(struct rp2040_pio_sm_config *c,
    uint32_t sideset_base)
{

	valid_params_if(PIO, sideset_base < 32);

	c->pinctrl = (c->pinctrl & ~RP2040_PIO_SM_PINCTRL_SIDESET_BASE_MASK) |
	    (sideset_base << RP2040_PIO_SM_PINCTRL_SIDESET_BASE_SHIFT);
}

void
rp2040_pio_get_default_sm_config(mdx_device_t dev,
    struct rp2040_pio_sm_config *c)
{

	rp2040_sm_config_set_clkdiv_int_frac(c, 1, 0);
	rp2040_sm_config_set_wrap(c, 0, 31);
	rp2040_sm_config_set_in_shift(c, true, false, 32);
	rp2040_sm_config_set_out_shift(c, true, false, 32);
}

void
rp2040_pio_sm_set_config(mdx_device_t dev, uint32_t sm,
    struct rp2040_pio_sm_config *config)
{
	struct rp2040_pio_softc *sc;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	WR4(sc, RP2040_PIO_SM_CLKDIV_OFFSET(sm), config->clkdiv);
	WR4(sc, RP2040_PIO_SM_EXECCTRL_OFFSET(sm), config->execctrl);
	WR4(sc, RP2040_PIO_SM_SHIFTCTRL_OFFSET(sm), config->shiftctrl);
	WR4(sc, RP2040_PIO_SM_PINCTRL_OFFSET(sm), config->pinctrl);
}

void
rp2040_pio_sm_clear_fifos(mdx_device_t dev, uint32_t sm)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	reg = RD4(sc, RP2040_PIO_SM_SHIFTCTRL_OFFSET(sm));
	reg ^= RP2040_PIO_SM_SHIFTCTRL_FJOIN_RX;
	WR4(sc, RP2040_PIO_SM_SHIFTCTRL_OFFSET(sm), reg);

	reg ^= RP2040_PIO_SM_SHIFTCTRL_FJOIN_RX;
	WR4(sc, RP2040_PIO_SM_SHIFTCTRL_OFFSET(sm), reg);
}

void
rp2040_pio_sm_set_enabled(mdx_device_t dev, int sm, int enable)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, RP2040_PIO_CTRL_OFFSET);
	if (enable)
		reg |= (1 << sm);
	else
		reg &= ~(1 << sm);
	WR4(sc, RP2040_PIO_CTRL_OFFSET, reg);
}

void
rp2040_pio_sm_restart(mdx_device_t dev, uint32_t sm)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	reg = (1 << (RP2040_PIO_CTRL_SM_RESTART_SHIFT + sm));
	WR4(sc, RP2040_PIO_CTRL_OFFSET, reg);
}

void
rp2040_pio_sm_clkdiv_restart(mdx_device_t dev, uint32_t sm)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	reg = (1 << (RP2040_PIO_CTRL_CLKDIV_RESTART_SHIFT + sm));
	WR4(sc, RP2040_PIO_CTRL_OFFSET, reg);
}

void
rp2040_pio_sm_exec(mdx_device_t dev, uint32_t sm, uint32_t instr)
{
	struct rp2040_pio_softc *sc;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	WR4(sc, RP2040_PIO_SM_INSTR_OFFSET(sm), instr);
}

void
rp2040_pio_sm_init(mdx_device_t dev, int sm, uint32_t pio_offset,
    struct rp2040_pio_sm_config *config)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	rp2040_pio_sm_set_enabled(dev, sm, false);
	rp2040_pio_sm_set_config(dev, sm, config);
	rp2040_pio_sm_clear_fifos(dev, sm);

	/* Disable FIFO debug. */
	reg = (1 << RP2040_PIO_FDEBUG_TXOVER_SHIFT) |
	    (1 << RP2040_PIO_FDEBUG_RXUNDER_SHIFT) |
	    (1 << RP2040_PIO_FDEBUG_TXSTALL_SHIFT) |
	    (1 << RP2040_PIO_FDEBUG_RXSTALL_SHIFT);
	WR4(sc, RP2040_PIO_FDEBUG_OFFSET, reg << sm);

	rp2040_pio_sm_restart(dev, sm);
	rp2040_pio_sm_clkdiv_restart(dev, sm);
	rp2040_pio_sm_exec(dev, sm, pio_encode_jmp(pio_offset));
}

void
rp2040_pio_set_input_sync_bypass(mdx_device_t dev, int pin, bool enable)
{
	struct rp2040_pio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, RP2040_PIO_INPUT_SYNC_BYPASS_OFFSET);
	if (enable)
		reg |= (1 << pin);
	else
		reg &= ~(1 << pin);
	WR4(sc, RP2040_PIO_INPUT_SYNC_BYPASS_OFFSET, reg);
}

void
rp2040_pio_sm_set_consecutive_pindirs(mdx_device_t dev, uint32_t sm,
    uint32_t pin, uint32_t count, bool is_out)
{
	struct rp2040_pio_softc *sc;
	uint32_t pinctrl_saved;
	uint32_t pindir_val;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	check_pio_param(pio);
	check_sm_param(sm);

	valid_params_if(PIO, pin < 32u);

	pinctrl_saved = RD4(sc, RP2040_PIO_SM_PINCTRL_OFFSET(sm));
	pindir_val = is_out ? 0x1f : 0;

	while (count > 5) {
		reg = (5u << RP2040_PIO_SM_PINCTRL_SET_COUNT_SHIFT) |
		    (pin << RP2040_PIO_SM_PINCTRL_SET_BASE_SHIFT);
		WR4(sc, RP2040_PIO_SM_PINCTRL_OFFSET(sm), reg);
		rp2040_pio_sm_exec(dev, sm, pio_encode_set(pio_pindirs,
		    pindir_val));
		count -= 5;
		pin = (pin + 5) & 0x1f;
	}

	reg = (count << RP2040_PIO_SM_PINCTRL_SET_COUNT_SHIFT) |
	    (pin << RP2040_PIO_SM_PINCTRL_SET_BASE_SHIFT);
	WR4(sc, RP2040_PIO_SM_PINCTRL_OFFSET(sm), reg);

	rp2040_pio_sm_exec(dev, sm, pio_encode_set(pio_pindirs, pindir_val));
	WR4(sc, RP2040_PIO_SM_PINCTRL_OFFSET(sm), pinctrl_saved);
}
