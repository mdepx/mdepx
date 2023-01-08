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
		WR4(sc, PIO_INSTR_MEM(offset + i), reg);
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
