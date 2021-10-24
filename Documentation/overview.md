# Overview

MDEPX is an RTOS for embedded applications written in C/Assembly.

MDEPX features a build system written in Python.

## Key features
  * Real-time
  * Priority-based time-sliced round-robin scheduling
  * Fully preemptible, cooperative scheduling optional
  * Tickless operation
  * Static/Dynamic memory allocation
  * Timed Mutexes, Semaphores
  * Symmetric Multiprocessing (SMP)
  * BSD libc included
  * Flat address space

## ISA supported
  * ARM Cortex-M family (ARMv7-M, ARMv8-M) SMP
  * CHERI128 Hybrid and Pure ABI capability system models
  * MIPS32, MIPS64
  * RISC-V machine or supervisor modes, rv32, rv64, SMP

## Platforms supported
  * Raspberry Pi Pico (SMP)
  * Nordic Semiconductor nRF5, nRF9
  * STMicroelectronics STM32
  * Microchip PIC32
  * SiFive
  * some others...

## Emulators support (see emul/)
  * qemu/riscv64c128 CHERI Pure Capability Mode
  * qemu/riscv64 SMP
  * qemu/riscv32 SMP
  * qemu/mips64c128 CHERI Hybrid and Pure Capability Modes
  * qemu/mips64
