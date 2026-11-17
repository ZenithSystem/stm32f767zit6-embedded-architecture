# STM32F767ZIT6 — Bare-Metal & HAL Firmware

Driver a livello di registro (CMSIS) e implementazioni HAL per le periferiche del microcontrollore **STM32F767ZIT6** su scheda **NUCLEO-F767ZI**.

## Specifiche Hardware

| Parametro | Valore | Note |
| :--- | :--- | :--- |
| **Core** | ARM Cortex-M7 r1p1 @ 216 MHz | Pipeline superscalare a 6 stadi, Branch Prediction (462 DMIPS) |
| **FPU** | FPv5-D16 (Single & Double Precision) | 32 registri SP / 16 registri DP |
| **Cache L1** | 16 KB I-Cache + 16 KB D-Cache | Gestione esplicita richiesta con DMA (`SCB_Clean/InvalidateDCache`) |
| **Flash** | 2 MB Dual-Bank | Accesso via bus AXI (`0x08000000`) o ITCM (`0x00200000` + ART Accelerator) |
| **SRAM** | 512 KB | 128 KB DTCM + 368 KB SRAM1 + 16 KB SRAM2 (+ 16 KB ITCM-RAM) |
| **Package** | LQFP144 | 114 I/O (tolleranti a 5V ove indicato nel datasheet) |

## Architettura Interna

<p align="center">
  <img src="docs/assets/stm32f767_block_diagram.png" alt="STM32F767ZIT6 Block Diagram" width="850">
</p>

*Block diagram STM32F767xx (STMicroelectronics DS11532 Rev 9, Fig. 2).*

## Struttura e Moduli

* `docs/` — Architettura Cortex-M7, memory map e linker script
* `modules/01_rcc_clock_tree/` — Configurazione PLL, HSE Bypass 8 MHz, Over-Drive Mode a 216 MHz, Flash latency (7 WS)
* `modules/02_gpio_exti/` — Registri GPIO (`MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`, `BSRR`), Alternate Functions, EXTI
* `modules/03_nvic_systick/` — NVIC priority grouping (`AIRCR`), preemption/subpriority, SysTick
* `modules/04_dma_architecture/` — DMA1/DMA2, mappatura stream/channel, FIFO, double-buffer, coerenza D-Cache
* `modules/05_timers_pwm/` — Timer basic (`TIM6`/`TIM7`), general-purpose (`TIM2`-`TIM5`), advanced (`TIM1`/`TIM8`), dead-time, input capture
* `modules/06_usart_uart/` — Oversampling, baudrate, DMA con Idle Line Detection e ring buffer
* `modules/07_spi_i2c/` — SPI full-duplex DMA (CPOL/CPHA), I2C (`TIMINGR`)
* `modules/08_adc_dac/` — SAR ADC 12-bit, scan mode, trigger da timer, DMA circolare, DAC
* `modules/09_fpu_cache_mpu/` — Configurazione MPU per regioni DMA non-cacheable, FPU FPv5-D16
* `modules/10_fmc_external_mem/` — Flexible Memory Controller (SDRAM e bus paralleli)
* `modules/11_ethernet_lwip/` — Ethernet MAC (RMII) con PHY LAN8742A, descrittori DMA in SRAM2, LwIP
* `modules/12_usb_otg_fs/` — USB OTG FS device, endpoint FIFO, classe CDC

## Pinout NUCLEO-F767ZI

| Funzione | Pin | Configurazione |
| :--- | :--- | :--- |
| **LD1 (Verde)** | `PB0` | Output Push-Pull (Active High) |
| **LD2 (Blu)** | `PB7` | Output Push-Pull (Active High) |
| **LD3 (Rosso)** | `PB14` | Output Push-Pull (Active High) |
| **User Button B1** | `PC13` | Input + `EXTI13` (Active High, pull-down esterno) |
| **ST-LINK VCP TX** | `PD8` | `USART3_TX` (`AF7`) |
| **ST-LINK VCP RX** | `PD9` | `USART3_RX` (`AF7`) |
| **HSE Clock In** | `PH0` | 8 MHz MCO da ST-LINK (`HSEBYP = 1`) |

## Build

Requisiti: `arm-none-eabi-gcc`, `cmake` (>= 3.20), `ninja`, `openocd`.

    cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake
    cmake --build build

## Documentazione STMicroelectronics

* **RM0410** — STM32F76xxx and STM32F77xxx Reference Manual
* **DS11532** — STM32F765xx, STM32F767xx, STM32F768Ax, STM32F769xx Datasheet
* **PM0253** — STM32F7 Series Cortex-M7 Programming Manual
* **ES0334** — STM32F76xxx and STM32F77xxx Device Errata
* **UM1974** — STM32 Nucleo-144 Boards User Manual
