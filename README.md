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

Osfive has no build system. You have to construct a GNU or BSD makefile for your application and list all the files you want to include to the build. See [example applications](https://github.com/osfive/).

# Contributing

Please submit patches on github.

Note that Osfive uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
