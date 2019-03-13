# Introduction

Osfive is an RTOS/SDK for microcontrollers.

## Key features
- Application-centric
- Multi-threading supported for ARM Cortex-M
- Fully preemptible
- Tickless operation
- Static/Dynamic memory allocation

## Supported libraries
- BSD libc library
- libaeabi
- libFLAC
- md5
- tinycrypt
- xz

## Supported platforms
- ARM Cortex-M
  * Maxim MAX32625
  * Maxim MAX32660
  * Nordic Semiconductor nRF52
  * [Nordic Semiconductor nRF9160](https://github.com/osfive/nrf9160)
  * NXP LPC43xx
  * STM32xx
- MIPS
  * Bluespec Extensible RISC Implementation (BERI)
  * Microchip PIC32
- RISC-V
  * Microsemi
  * [SiFive E300G](https://github.com/osfive/hifive1-oled)
  * [GFE P1 Processor (Bluespec Piccolo)](https://github.com/CTSRD-CHERI/piccolo-demo)

# Getting started

Osfive has no build system. You have to construct your own GNU or BSD makefile listing all the files you want to include to the build. See [example applications](https://github.com/osfive/).

# Contributing

Please submit patches on github.

Note osfive uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
