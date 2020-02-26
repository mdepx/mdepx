# Overview

MDEPX is an RTOS for embedded applications written in C/Assembly.

MDEPX features a build system written in Python.

## Key features
- Real-time
- Priority-based time-sliced round-robin scheduling
- Fully preemptible
- Cooperative scheduling
- Tickless operation
- Static/Dynamic memory allocation
- Timed Mutexes, Semaphores
- Symmetric Multiprocessing (SMP)
- BSD libc included

## Supported CPU architectures
- ARM Cortex-M family (ARMv7-M, ARMv8-M)
- MIPS32, MIPS64
- RISC-V rv32, rv64
- CHERI128

## Generic demo applications
  * QEMU/RISC-V rv64gc (SMP)
  * QEMU/MIPS64
  * QEMU/CHERI128
  * HiFive Unleashed (SMP)
  * HiFive Unleashed Monitor Core
