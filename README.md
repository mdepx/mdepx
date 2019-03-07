# Introduction

Osfive is an RTOS/SDK for microcontrollers.

## Key features
- Application-centric
- Single-threaded
- Tickless operation
- Static memory allocation (malloc() is provided for applications)

## Supported libraries
- BSD libc library
- libaeabi
- libFLAC
- md5
- tinycrypt
- xz

## Supported platforms
- ARM
  * Maxim MAX32625
  * Maxim MAX32660
  * Nordic Semiconductor nRF52
  * Nordic Semiconductor nRF9160
  * NXP LPC43xx
  * STM32xx
- MIPS
  * Bluespec Extensible RISC Implementation (BERI)
  * Microchip PIC32
- RISC-V
  * Microsemi
  * SiFive E300G
  * GFE P1 Processor (Bluespec Piccolo)

# Getting started

Osfive has no build system. You have to construct your own GNU or BSD makefile listing all the files you want to include to the build. See [example applications](https://github.com/osfive/).

# Contributing

Osfive uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
