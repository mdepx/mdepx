#ifndef _ARM_RASPBERRYPI_RP2040_PIO_H_
#define _ARM_RASPBERRYPI_RP2040_PIO_H_

struct rp2040_pio_softc {
	uint32_t base;
	uint32_t used_instruction_space;
};

#define	PIO_INSTR_MEM(x)	(0x48 + ((x) * 0x4))

typedef uint32_t uint;

typedef struct rp2040_pio_program {
	const uint16_t *instructions;
	uint8_t length;
	int8_t origin; // required instruction memory origin or -1
} __packed rp2040_pio_program_t;

#define	PIO_INSTRUCTION_COUNT		32
#define	PARAM_ASSERTIONS_ENABLED(x)	0

#define valid_params_if(x, y)

bool pio_can_add_program(mdx_device_t, const rp2040_pio_program_t *program);
uint pio_add_program(mdx_device_t, const rp2040_pio_program_t *program);
int rp2040_pio_init(mdx_device_t dev, uint32_t base);

#endif /* !_ARM_RASPBERRYPI_RP2040_PIO_H_ */
