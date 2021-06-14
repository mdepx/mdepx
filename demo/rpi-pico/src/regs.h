/* SPDX-License-Identifier: MIT */
#ifndef REGS_H
#define REGS_H

#define OP_RW  (0x0000)
#define OP_XOR (0x1000)
#define OP_SET (0x2000)
#define OP_CLR (0x3000)

#define RESETS_BASE                 (0x4000C000)

#define RESETS_RESET             (RESETS_BASE+0x0)
#define RESETS_RESET_DONE        (RESETS_BASE+0x8)

#define SIO_BASE (0xD0000000)

#define SIO_GPIO_OUT_SET (SIO_BASE+0x14)
#define SIO_GPIO_OUT_CLR (SIO_BASE+0x18)
#define SIO_GPIO_OUT_XOR (SIO_BASE+0x1C)
#define SIO_GPIO_OE_SET (SIO_BASE+0x24)
#define SIO_GPIO_OE_CLR (SIO_BASE+0x28)

#define PADS_BANK0_BASE (0x4001C000)
#define PADS_BANK0_GPIO (0x68)

#define PADS_GPIO25 (PADS_BANK0_BASE + PADS_BANK0_GPIO)

#define IO_BANK0_BASE (0x40014000)
#define IO_BANK_GPIO25_CTRL (0xCC)

#define IO_GPIO25_CTRL (IO_BANK0_BASE + IO_BANK_GPIO25_CTRL)


#endif //REGS_H
