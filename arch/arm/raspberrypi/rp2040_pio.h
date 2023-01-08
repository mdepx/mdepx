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
#define	check_pio_param(x)
#define	check_sm_param(x)

bool pio_can_add_program(mdx_device_t, const rp2040_pio_program_t *program);
uint pio_add_program(mdx_device_t, const rp2040_pio_program_t *program);
int rp2040_pio_init(mdx_device_t dev, uint32_t base);

struct rp2040_pio_sm_config {
	uint32_t clkdiv;
	uint32_t execctrl;
	uint32_t shiftctrl;
	uint32_t pinctrl;
};

enum rp2040_pio_fifo_join {
	RP2040_PIO_FIFO_JOIN_NONE = 0,
	RP2040_PIO_FIFO_JOIN_TX = 1,
	RP2040_PIO_FIFO_JOIN_RX = 2,
};

enum rp2040_pio_mov_status_type {
	RP2040_STATUS_TX_LESSTHAN = 0,
	RP2040_STATUS_RX_LESSTHAN = 1
};

void rp2040_sm_config_set_clkdiv_int_frac(struct rp2040_pio_sm_config *c,
    uint16_t div_int, uint8_t div_frac);

void rp2040_sm_config_set_wrap(struct rp2040_pio_sm_config *c,
    uint32_t wrap_target, uint32_t wrap);

void rp2040_sm_config_set_in_shift(struct rp2040_pio_sm_config *c,
    bool shift_right, bool autopush, uint32_t push_threshold);

void rp2040_sm_config_set_out_shift(struct rp2040_pio_sm_config *c,
    bool shift_right, bool autopull, uint32_t pull_threshold);

void rp2040_sm_config_set_out_pins(struct rp2040_pio_sm_config *c,
    uint32_t out_base, uint32_t out_count);

void rp2040_pio_get_default_sm_config(mdx_device_t dev,
    struct rp2040_pio_sm_config *config);

void rp2040_sm_config_set_in_pins(struct rp2040_pio_sm_config *c,
    uint32_t in_base);

void rp2040_sm_config_set_set_pins(struct rp2040_pio_sm_config *c,
    uint32_t set_base, uint32_t set_count);

void rp2040_sm_config_set_sideset(struct rp2040_pio_sm_config *c,
    uint32_t bit_count, bool optional, bool pindirs);

void rp2040_sm_config_set_sideset_pins(struct rp2040_pio_sm_config *c,
    uint32_t sideset_base);

#endif /* !_ARM_RASPBERRYPI_RP2040_PIO_H_ */
