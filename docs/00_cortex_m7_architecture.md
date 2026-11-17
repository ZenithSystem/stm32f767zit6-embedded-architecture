# 00 — Architettura Interna ARM Cortex-M7 e Bus Matrix STM32F767

## 1. Perché il Cortex-M7 è diverso da M3/M4
A differenza dei core Cortex-M3 e M4 (pipeline a 3 stadi, bus AHB-Lite), l'**ARM Cortex-M7** introduce un'architettura ad alte prestazioni progettata per DSP e controllo in tempo reale avanzato:
* **Pipeline superscalare a 6 stadi** con dual-issue (può eseguire due istruzioni per ciclo di clock in condizioni ottimali).
* **Branch Prediction** dinamica con Branch Target Address Cache (BTAC).
* **Bus AXI a 64-bit** verso la matrice di interconnessione principale, affiancato da bus dedicati **ITCM** (Instruction Tightly-Coupled Memory, 64-bit) e **DTCM** (Data Tightly-Coupled Memory, 2x32-bit).

## 2. Il Problema Critico della Coerenza della Cache (D-Cache vs DMA)
Sul STM32F767ZIT6, il core accede alla SRAM1/SRAM2 passando attraverso la **Data Cache (16 KB)**, mentre i controller **DMA1 / DMA2 / Ethernet / USB** accedono direttamente alla memoria fisica tramite la **Bus Matrix** senza passare dalla cache del core.

* **Scenario TX (CPU -> DMA -> Periferica):** La CPU scrive i dati in un buffer in `SRAM1`, ma il dato resta *dirty* nella D-Cache. Il DMA legge dalla `SRAM1` fisica e trasmette dati vecchi!
  * **Soluzione:** `SCB_CleanDCache_by_Addr((uint32_t*)tx_buf, size);` prima di avviare il DMA.
* **Scenario RX (Periferica -> DMA -> CPU):** Il DMA scrive nuovi dati nella `SRAM1` fisica, ma la CPU legge una copia obsoleta presente nella D-Cache!
  * **Soluzione:** `SCB_InvalidateDCache_by_Addr((uint32_t*)rx_buf, size);` prima che la CPU legga il buffer.
* **Alternativa Deterministica (Zero Overhead):** Allocare i buffer DMA in **DTCM-RAM (`0x20000000`)** (non cachata, sempre coerente a zero wait-state) oppure configurare una regione **MPU** non-cacheable in `SRAM2`.
