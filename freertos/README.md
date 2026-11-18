# FreeRTOS — Multitasking Real-Time su STM32F767ZIT6

Spazio dedicato agli esempi di programmazione concorrente e deterministica con **FreeRTOS** su core **ARM Cortex-M7**:

* Creazione e schedulazione di task con priorità differenziate e periodi deterministici (`vTaskDelayUntil`).
* Sincronizzazione tra interrupt hardware (ISR) e task tramite `Queue`, `Semaphore` e `Task Notifications`.
* Gestione delle risorse condivise tramite `Mutex` con priority inheritance e configurazione `FreeRTOSConfig.h`.
