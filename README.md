# Introduction

Mdepx is an operating system for embedded applications.

## Key features
- Real-time
- Priority based time sliced round-robin scheduling
- Fully preemptible
- Cooperative scheduling
- Tickless operation
- Static/Dynamic memory allocation
- Timed Mutexes, Semaphores
- Symmetric Multiprocessing (SMP)
- BSD libc included

## Supported platforms
- ARM Cortex-M family (ARMv7-M, ARMv8-M)
- MIPS32, MIPS64
- RISC-V rv32, rv64

## Generic demo applications
  * QEMU/RISC-V rv64ima (SMP)
  * QEMU/MIPS64
  * QEMU/CHERI128
  * HiFive Unleashed (SMP)
  * HiFive Unleashed Monitor Core

## Featured applications (external)
  * [LTE 4G Link with nRF9160](https://github.com/machdep/nrf9160)
  * [Bluetooth LE with nRF5340](https://github.com/machdep/nrf5340)
  * CTSRD CHERI [device-model](https://github.com/CTSRD-CHERI/device-model)
  * [Driving MIPI DSI Display with STM32F469I-DISCO](https://github.com/machdep/stm32f469i-disco-dsi)
  * [Driving OLED display on HiFive1 board](https://github.com/machdep/hifive1-oled)
  * GFE (Government Furnished Equipment) [Bluespec Piccolo demo](https://github.com/CTSRD-CHERI/piccolo-demo)
  * [Driving IPS Display with PIC32MZDA Starter Kit](https://github.com/osfive/pic32mzdask-ips)

# Getting started

Mdepx has a python3-based build system. You have to construct a configuration file for your application. See [demo applications](https://github.com/machdep/mdepx/tree/master/demo).

# Contributing

Please submit pull requests on github or send patches to br@machdep.com.

Note that mdepx uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
