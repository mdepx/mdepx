# Introduction

mdepx (Machdep Extended) is an operating system for embedded applications.

## Key features
- Real-time
- Priority-based time-sliced round-robin scheduling
- Fully preemptible, optionally cooperative scheduling
- Tickless operation
- Static/Dynamic memory allocation
- Timed Mutexes, Semaphores
- Symmetric Multiprocessing (SMP)
- BSD libc included
- Flat address space

## Supported platforms
- ARM Cortex-M family (ARMv7-M, ARMv8-M) SMP
- CHERI128 Hybrid and Pure ABI capability system models
- MIPS32, MIPS64
- RISC-V rv32, rv64 SMP

## Emulators support (see emul/)
  * QEMU/CHERI128 hybrid and pure capability modes
  * QEMU/MIPS64
  * QEMU/RISC-V rv64gc SMP

## Limitations
  * Generic exception handlers are not re-entrant (have your own vectors for nested traps if needed)

## Featured applications (external)
  * [SMP on Raspberry Pi Pico](https://github.com/machdep/raspberrypi-pico)
  * [LTE 4G Link with nRF9160](https://github.com/machdep/nrf9160)
  * CTSRD-CHERI [device-model](https://github.com/CTSRD-CHERI/device-model) [device-model-riscv](https://github.com/CTSRD-CHERI/device-model-riscv)
  * Other [example apps](https://github.com/machdep/)

# Getting started

See [documentation](https://machdep.uk).

# Contributing

Please submit pull requests on github or send patches to br@machdep.com.

Note that mdepx uses FreeBSD [style(9)](https://www.freebsd.org/cgi/man.cgi?query=style&sektion=9) guide.

# License

This project is licensed under two-clause [BSD license](https://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_(%22Simplified_BSD_License%22_or_%22FreeBSD_License%22)).
