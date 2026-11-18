# STM32F767ZIT6 — Architettura Embedded & Firmware

![C](https://img.shields.io/badge/Language-C11-00599C?style=flat-square&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![ARM](https://img.shields.io/badge/Architecture-ARM_Cortex--M-0091BD?style=flat-square&logo=arm&logoColor=white)
![ARM Assembly](https://img.shields.io/badge/Low--Level-ARM_Assembly-374151?style=flat-square&logo=arm&logoColor=white)
![STM32](https://img.shields.io/badge/MCU-STM32-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-009639?style=flat-square&logo=freertos&logoColor=white)
![Zephyr](https://img.shields.io/badge/RTOS-Zephyr-7829AD?style=flat-square&logo=zephyr&logoColor=white)
![Arduino](https://img.shields.io/badge/Platform-Arduino-00878F?style=flat-square&logo=arduino&logoColor=white)
![Bare-Metal](https://img.shields.io/badge/Firmware-Bare--Metal-B91C1C?style=flat-square)
![GCC](https://img.shields.io/badge/Toolchain-ARM_GCC-A42E2B?style=flat-square&logo=gnu&logoColor=white)

## Panoramica

Questo repository costituisce un punto di riferimento pratico e un ambiente di studio per comprendere l'architettura interna e il funzionamento delle periferiche del microcontrollore **STM32F767ZIT6** (ARM Cortex-M7) su scheda **NUCLEO-F767ZI**.

Raccoglie esempi di codice autonomi e documentati dedicati alla gestione dell'hardware, all'inizializzazione delle periferiche, al controllo deterministico dei tempi e al multitasking real-time su quattro livelli:

* **Driver Bare-Metal (livello registro):** manipolazione diretta dei registri memory-mapped tramite puntatori `volatile`, ricavati dal Reference Manual ST (**RM0410**), senza livelli di astrazione intermedi.
* **Controparti STM32 HAL (`*_hal.c`):** implementazioni parallele sviluppate con la libreria ufficiale **STM32CubeF7 HAL** per confrontare il controllo diretto dei registri con le API del produttore.
* **FreeRTOS (Multitasking Real-Time):** schedulazione preemptive di task, sincronizzazione inter-task tramite code, semafori e mutex, e gestione deterministica delle priorità su Cortex-M7.
* **Zephyr RTOS (Prototipazione Avionica):** sviluppo di applicazioni real-time multi-thread e DeviceTree per la prototipazione di sottosistemi avionici (telemetria, acquisizione sensori inerziali/barometrici, bus di comunicazione e controllo deterministico).

## Architettura Hardware

<img src="docs/assets/stm32f767_block_diagram.png" alt="STM32F767ZIT6 Block Diagram" width="850">

## FreeRTOS — Multitasking Deterministico

L'integrazione di **FreeRTOS** sull'**STM32F767ZIT6** consente di studiare e implementare architetture firmware concorrenti e deterministiche sia tramite API native (`task.h`, `queue.h`, `semphr.h`) sia tramite wrapper **CMSIS-RTOS v2**:

* **Gestione Task & Scheduler Preemptive:** creazione di task con priorità dedicate, temporizzazione periodica precisa con `vTaskDelayUntil()` e controllo dell'utilizzo dello stack.
* **Sincronizzazione e Comunicazione Inter-Task:** trasferimento sicuro di dati tra ISR e task tramite `Queue`, protezione delle risorse condivise con `Mutex` (priority inheritance) e segnalazione eventi con `Binary/Counting Semaphore` e `Task Notifications`.
* **Gestione Memoria & Tick:** configurazione del gestore dinamico `heap_4.c`, interrupt `SysTick` / `PendSV` / `SVC` del core ARM Cortex-M7 e priorità NVIC compatibili con `configMAX_SYSCALL_INTERRUPT_PRIORITY`.

## Zephyr RTOS — Prototipazione di Avionica

La scheda **NUCLEO-F767ZI** (`nucleo_f767zi`) è supportata nativamente dall'ecosistema **Zephyr RTOS**, utilizzato in questo repository come ambiente per la prototipazione rapida e deterministica di firmware avionico:

* **Scheduling Real-Time & Multi-Threading:** separazione deterministica dei task critici (loop di controllo assetto, fusione sensori IMU, telemetria, logging) tramite thread preemptive e cooperativi, timer kernel, semafori e message queue.
* **Astrazione Hardware tramite DeviceTree (`.dts` / `.overlay`):** configurazione dichiarativa di pinout, bus **SPI**, **I2C**, **UART** e **CAN** disaccoppiata dal codice applicativo, facilitando il porting tra prototipi su scheda di sviluppo e hardware custom.
* **Sottosistemi & Driver Integrati:** utilizzo nativo di `Sensor API`, ring buffer, gestione energetica e stack di comunicazione per sensori di volo (accelerometri, giroscopi, magnetometri, barometri) e collegamenti di telemetria.

## Struttura del Repository

| Percorso | Ambito | Argomenti Principali |
| :--- | :--- | :--- |
| `modules/01_rcc_clock_tree/` | Reset & Clock Control / Base Tempi | Abilitazione clock su bus (`AHB1ENR`), timer hardware `SysTick`, polling non-bloccante, `HAL_GetTick()` |
| `modules/02_gpio_exti/` | General-Purpose I/O & Interrupt | Configurazione modalità pin (`MODER`), controllo uscite (`ODR`/`BSRR`), delay assembly `NOP`, GPIO HAL |
| `freertos/` | FreeRTOS & Concorrenza Real-Time | Task preemptive, `Queue`, `Mutex`, `Semaphore`, sincronizzazione ISR-to-Task, `heap_4` |
| `zephyr_avionics/` | Zephyr RTOS & Prototipazione Avionica | Progetti `west`, overlay DeviceTree (`nucleo_f767zi`), thread real-time, acquisizione sensori e telemetria |
| `common/` | Definizioni Condivise & Startup | Indirizzi base di memoria, linker script e definizioni hardware comuni |

## Esempi Implementati

| File | Approccio | Descrizione |
| :--- | :--- | :--- |
| `modules/02_gpio_exti/01_led_blink.c` | Bare-Metal | Blink bloccante del LED LD1 (`PB0`) tramite registri `RCC_AHB1ENR`, `GPIOB_MODER` e `GPIOB_ODR` |
| `modules/02_gpio_exti/01_led_blink_hal.c` | STM32 HAL | Controparte HAL con `HAL_GPIO_TogglePin()` e `HAL_Delay()` |
| `modules/02_gpio_exti/02_led_blink_nop.c` | Bare-Metal + Assembly | Temporizzazione tramite istruzione inline assembly `__asm__ volatile ("nop")` |
| `modules/02_gpio_exti/02_led_blink_nop_hal.c` | STM32 HAL + Assembly | Inizializzazione GPIO via HAL e ciclo di attesa deterministico con istruzione `NOP` |
| `modules/01_rcc_clock_tree/03_led_blink_nonblocking.c` | Bare-Metal | Base tempi non-bloccante a 1 ms tramite polling su `SYST_CSR_COUNTFLAG` del timer `SysTick` |
| `modules/01_rcc_clock_tree/03_led_blink_nonblocking_hal.c` | STM32 HAL | Base tempi non-bloccante tramite `HAL_GetTick()` e interrupt `SysTick_Handler()` |

## Toolchain e Flusso di Lavoro

* **Compilatore Bare-Metal / HAL / FreeRTOS:** `arm-none-eabi-gcc` (`-std=c11`, `-mcpu=cortex-m7`, `-mthumb`)
* **Build System Zephyr RTOS:** `west` (Zephyr meta-tool, target `-b nucleo_f767zi`)
* **Flashing e Debug:** `openocd` (interfaccia `stlink` e target `stm32f7x`)
* **Formattazione Codice:** `clang-format` applicato localmente tramite hook Git `pre-commit` e verificato ad ogni push da GitHub Actions
