/*
 * @file    stm32f767_regs.h
 * @brief   Definizioni memory-mapped dei registri RCC, GPIO e SysTick (RM0410 / ARMv7-M).
 */

#ifndef STM32F767_REGS_H
#define STM32F767_REGS_H

#include <stdint.h>

// Base addresses (RM0410 Section 2.2.2, Table 1)
#define PERIPH_BASE     (0x40000000UL)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define GPIOB_BASE      (AHB1PERIPH_BASE + 0x0400UL)
#define RCC_BASE        (AHB1PERIPH_BASE + 0x3800UL)

// ARM Cortex-M7 System Control Space (ARMv7-M TRM Section B3.3)
#define SCS_BASE     (0xE000E000UL)
#define SYSTICK_BASE (SCS_BASE + 0x0010UL)

typedef struct
{
    volatile uint32_t CR;       // 0x00: Clock control register
    volatile uint32_t PLLCFGR;  // 0x04: PLL configuration register
    volatile uint32_t CFGR;     // 0x08: Clock configuration register
    volatile uint32_t CIR;      // 0x0C: Clock interrupt register
    volatile uint32_t AHB1RSTR; // 0x10: AHB1 peripheral reset register
    volatile uint32_t AHB2RSTR; // 0x14: AHB2 peripheral reset register
    volatile uint32_t AHB3RSTR; // 0x18: AHB3 peripheral reset register
    uint32_t RESERVED0;         // 0x1C
    volatile uint32_t APB1RSTR; // 0x20: APB1 peripheral reset register
    volatile uint32_t APB2RSTR; // 0x24: APB2 peripheral reset register
    uint32_t RESERVED1[2];      // 0x28 - 0x2C
    volatile uint32_t AHB1ENR;  // 0x30: AHB1 peripheral clock enable register
} RCC_TypeDef;

typedef struct
{
    volatile uint32_t MODER;   // 0x00: Port mode register
    volatile uint32_t OTYPER;  // 0x04: Port output type register
    volatile uint32_t OSPEEDR; // 0x08: Port output speed register
    volatile uint32_t PUPDR;   // 0x0C: Port pull-up/pull-down register
    volatile uint32_t IDR;     // 0x10: Port input data register
    volatile uint32_t ODR;     // 0x14: Port output data register
    volatile uint32_t BSRR;    // 0x18: Port bit set/reset register
    volatile uint32_t LCKR;    // 0x1C: Port configuration lock register
    volatile uint32_t AFR[2];  // 0x20 - 0x24: Alternate function registers
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;  // 0x00: Control and status register
    volatile uint32_t LOAD;  // 0x04: Reload value register (24-bit)
    volatile uint32_t VAL;   // 0x08: Current value register
    volatile uint32_t CALIB; // 0x0C: Calibration value register
} SysTick_TypeDef;

#define RCC     ((RCC_TypeDef *)RCC_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define SysTick ((SysTick_TypeDef *)SYSTICK_BASE)

// RCC_AHB1ENR bits
#define RCC_AHB1ENR_GPIOBEN (1UL << 1U)

// SysTick_CTRL bits
#define SYST_CSR_ENABLE    (1UL << 0U)
#define SYST_CSR_CLKSOURCE (1UL << 2U)
#define SYST_CSR_COUNTFLAG (1UL << 16U)

#endif // STM32F767_REGS_H
