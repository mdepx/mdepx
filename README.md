# Introduction

Osfive is an RTOS/SDK for microcontrollers.

## Key features
- Single-threaded
- Application-centric
- Tickless operation

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
  * STM32
- MIPS
  * Microchip PIC32
  * BERI
- RISC-V
  * Microsemi
  * SiFive E300G

# Getting started

Osfive has no build system. You have to construct your own GNU or BSD makefile listing all the files you want to include to the build. See [example applications](https://github.com/osfive/).

# Contributing

Osfive uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
