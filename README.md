# STM32F767ZIT6 Embedded Architecture

![C](https://img.shields.io/badge/Language-C11-00599C?style=flat-square&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![ARM](https://img.shields.io/badge/Architecture-ARM_Cortex--M-0091BD?style=flat-square&logo=arm&logoColor=white)
![ARM Assembly](https://img.shields.io/badge/Low--Level-ARM_Assembly-374151?style=flat-square&logo=arm&logoColor=white)
![STM32](https://img.shields.io/badge/MCU-STM32-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![Arduino](https://img.shields.io/badge/Platform-Arduino-00878F?style=flat-square&logo=arduino&logoColor=white)
![Bare-Metal](https://img.shields.io/badge/Firmware-Bare--Metal-B91C1C?style=flat-square)
![GCC](https://img.shields.io/badge/Toolchain-ARM_GCC-A42E2B?style=flat-square&logo=gnu&logoColor=white)

## Overview

This repository serves as a practical reference and learning workspace for studying the internal architecture and peripheral operation of the **STM32F767ZIT6** microcontroller (ARM Cortex-M7) on the **NUCLEO-F767ZI** board.

It collects self-contained, documented code examples focused on hardware management, peripheral initialization, clock configuration, and deterministic execution:

* **Bare-Metal Drivers (Register-Level):** Direct manipulation of memory-mapped peripheral registers via `volatile` pointers, strictly derived from the ST Reference Manual (**RM0410**), with zero abstraction overhead.
* **STM32 HAL Counterparts (`*_hal.c`):** Parallel reference implementations built with the official **STM32CubeF7 HAL** to compare direct register control against vendor driver APIs.

## Hardware Architecture

<img src="docs/assets/stm32f767_block_diagram.png" alt="STM32F767ZIT6 Block Diagram" width="850">

## Repository Structure

| Path | Scope | Key Topics |
| :--- | :--- | :--- |
| `modules/01_rcc_clock_tree/` | Reset & Clock Control / Timebase | Bus clock gating (`AHB1ENR`), SysTick hardware timer, non-blocking polling, `HAL_GetTick()` |
| `modules/02_gpio_exti/` | General-Purpose I/O & Interrupts | Pin mode configuration (`MODER`), output control (`ODR`/`BSRR`), NOP assembly delays, HAL GPIO |
| `common/` | Shared Definitions & Startup | Base memory addresses, linker scripts, and common hardware definitions |

## Toolchain & Workflow

* **Compiler:** `arm-none-eabi-gcc` (`-std=c11`, `-mcpu=cortex-m7`, `-mthumb`)
* **Flashing & Debug:** `openocd` (`stlink` interface + `stm32f7x` target)
* **Code Formatting:** `clang-format` enforced locally via Git `pre-commit` hook and verified on every push by GitHub Actions
