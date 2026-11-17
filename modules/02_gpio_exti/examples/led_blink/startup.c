/*
 * @file    startup.c
 * @brief   Vector table e Reset_Handler per ARM Cortex-M7 (STM32F767ZIT6).
 */

#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

typedef void (*isr_handler_t)(void);

// Prime 16 eccezioni architetturali Cortex-M7 (RM0410 Section 10.1.2)
__attribute__((section(".isr_vector"), used)) const isr_handler_t vector_table[16] = {
    (isr_handler_t)&_estack, // 0x00: Initial Main Stack Pointer
    Reset_Handler,           // 0x04: Reset
    Default_Handler,         // 0x08: NMI
    Default_Handler,         // 0x0C: HardFault
    Default_Handler,         // 0x10: MemManage
    Default_Handler,         // 0x14: BusFault
    Default_Handler,         // 0x18: UsageFault
    0,                       // 0x1C: Reserved
    0,                       // 0x20: Reserved
    0,                       // 0x24: Reserved
    0,                       // 0x28: Reserved
    Default_Handler,         // 0x2C: SVCall
    Default_Handler,         // 0x30: Debug Monitor
    0,                       // 0x34: Reserved
    Default_Handler,         // 0x38: PendSV
    Default_Handler          // 0x3C: SysTick
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    // Copia sezione .data da Flash a DTCM-RAM
    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    // Azzera sezione .bss in DTCM-RAM
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0U;
    }

    (void)main();

    while (1)
    {
    }
}

void Default_Handler(void)
{
    while (1)
    {
    }
}
