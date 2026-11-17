# LED Blink — Bare-Metal C (Zero Dependencies)

Accensione sequenziale dei tre LED utente della **NUCLEO-F767ZI** senza librerie ST (HAL/LL) né header CMSIS esterni.

## Struttura

* `stm32f767_regs.h` — Registri `RCC`, `GPIOB` e `SysTick` mappati agli indirizzi fisici (`RM0410` §2.2.2).
* `startup.c` — Vector table minimale Cortex-M7 e `Reset_Handler` (copia `.data` in DTCM-RAM, azzera `.bss`).
* `STM32F767ZITx.ld` — Linker script (`FLASH` @ `0x08000000`, `DTCMRAM` @ `0x20000000`).
* `main.c` — Abilitazione clock `GPIOB`, configurazione output Push-Pull su `PB0`/`PB7`/`PB14` e delay 1 ms tramite `SysTick` (HSI 16 MHz).
* `Makefile` — Build con `arm-none-eabi-gcc` e flashing via `openocd`.

## Mappatura Pin e Registri

| LED | Pin | `MODER` | `BSRR` Set | `BSRR` Reset |
| :--- | :--- | :--- | :--- | :--- |
| **LD1 (Verde)** | `PB0` | `MODER[1:0] = 01` | Bit `0` | Bit `16` |
| **LD2 (Blu)** | `PB7` | `MODER[15:14] = 01` | Bit `7` | Bit `23` |
| **LD3 (Rosso)** | `PB14` | `MODER[29:28] = 01` | Bit `14` | Bit `30` |

## Build & Flash

    make
    make flash
