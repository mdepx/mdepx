# Introduction

Mdepx is an operating system for embedded applications.

## Key features
- Real-time
- Priority based time sliced round-robin scheduling
- Fully preemptible
- Tickless operation
- Static/Dynamic memory allocation
- Timed Mutexes, Semaphores
- Symmetric Multiprocessing (SMP)
- BSD libc included

## Supported platforms
- ARM Cortex-M family (ARMv7, ARMv8)
- MIPS32, MIPS64
- RISC-V rv32ima, rv64ima

## Generic demo applications
  * QEMU/RISC-V rv64ima (SMP)
  * QEMU/MIPS64
  * HiFive Unleashed (SMP)
  * HiFive Unleashed Monitor Core

## Featured applications (external)
  * CTSRD CHERI [device-model](https://github.com/CTSRD-CHERI/device-model)
  * [Driving OLED display on HiFive1 board](https://github.com/osfive/hifive1-oled)
  * [Setting up LTE 4G Link with nRF9160](https://github.com/osfive/nrf9160)
  * GFE (Government Furnished Equipment) [Bluespec Piccolo demo](https://github.com/CTSRD-CHERI/piccolo-demo)
  * [Driving MIPI DSI Display with STM32F469I-DISCO](https://github.com/osfive/stm32f469i-disco-dsi)
  * [Driving IPS Display with PIC32MZDA Starter Kit](https://github.com/osfive/pic32mzdask-ips)

# Getting started

Mdepx has a Makefile-based build system. You have to construct a GNU makefile for your application. See [example applications](https://github.com/osfive/).

# Contributing

Please submit pull requests on github or send patches to br@machdep.com.

Note that Mdepx uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
