# Introduction

Osfive is a framework for building bare-metal applications.

## Key features
- Application-centric
- Multi-threading supported
- Fully preemptible
- Tickless operation
- Static/Dynamic memory allocation
- Mutexes

## Supported platforms
- ARM Cortex-M
- MIPS (32 and 64 bit)
- RISC-V RV32

## Included libraries
- BSD libc library
- libaeabi
- libFLAC
- libfont
- md5
- mbedtls
- tinycrypt
- xz

## Featured applications
  * CTSRD CHERI [device-model](https://github.com/CTSRD-CHERI/device-model)
  * [Driving OLED display on HiFive1 board](https://github.com/osfive/hifive1-oled)
  * [Setting up LTE 4G Link with nRF9160](https://github.com/osfive/nrf9160)
  * GFE (Government Furnished Equipment) [Bluespec Piccolo demo](https://github.com/CTSRD-CHERI/piccolo-demo)
  * [Driving MIPI DSI Display with STM32F469I-DISCO](https://github.com/osfive/stm32f469i-disco-dsi)
  * [Driving IPS Display with PIC32MZDA Starter Kit](https://github.com/osfive/pic32mzdask-ips)

# Getting started

Osfive has a Makefile-based build system. You have to construct a GNU makefile for your application. See [example applications](https://github.com/osfive/).

Basic rules for the makefiles:
  * Define APP name, MACHINE you target, KERNEL options and list of LIBRARIES.
  * Define list of OBJECTS including your app files and kernel drivers.
  * Provide an LDSCRIPT.
  * Define a toolchain paths to CC, LD and OBJCOPY.
  * Include gnu.mk

## Kernel options
  * sched - includes round-robin scheduler and threads management code
  * malloc - includes free-list memory allocator

# Contributing

Please submit pull requests on github or send patches to br@bsdpad.com.

Note that Osfive uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
