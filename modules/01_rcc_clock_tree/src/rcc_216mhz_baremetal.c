/**
 * @file    rcc_216mhz_baremetal.c
 * @brief   Configurazione Bare-Metal (CMSIS) del Clock Tree STM32F767ZIT6 a 216 MHz.
 * @target  NUCLEO-F767ZI (HSE = 8 MHz MCO da ST-LINK V2-1)
 * @ref     RM0410 Rev 4 - Sezione 5 (Reset and Clock Control)
 */

#include "stm32f767xx.h"

void SystemClock_Config_216MHz_BareMetal(void)
{
    /* 1. Abilita clock per Power Interface (PWR) */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    (void)RCC->APB1ENR;

    /* 2. Imposta Voltage Regulator Scale 1 (necessario per f > 180 MHz) */
    PWR->CR1 |= PWR_CR1_VOS;

    /* 3. Avvia HSE in modalita' Bypass (clock quadro 8 MHz da ST-LINK su PH0) */
    RCC->CR |= RCC_CR_HSEBYP;
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0U)
    {
    }

    /* 4. Configura Main PLL: PLLM=4, PLLN=216, PLLP=2, PLLQ=9, sorgente HSE */
    RCC->PLLCFGR = (4U << RCC_PLLCFGR_PLLM_Pos) | (216U << RCC_PLLCFGR_PLLN_Pos) |
                   (0U << RCC_PLLCFGR_PLLP_Pos) | (9U << RCC_PLLCFGR_PLLQ_Pos) |
                   RCC_PLLCFGR_PLLSRC_HSE;

    /* 5. Abilita il Main PLL e attendi il lock */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U)
    {
    }

    /* 6. Attiva Over-Drive Mode per raggiungere 216 MHz */
    PWR->CR1 |= PWR_CR1_ODEN;
    while ((PWR->CSR1 & PWR_CSR1_ODRDY) == 0U)
    {
    }

    PWR->CR1 |= PWR_CR1_ODSWEN;
    while ((PWR->CSR1 & PWR_CSR1_ODSWRDY) == 0U)
    {
    }

    /* 7. Configura Flash Latency (7 Wait States a 216 MHz @ 3.3V) + ART Accelerator */
    FLASH->ACR = FLASH_ACR_LATENCY_7WS | FLASH_ACR_ARTEN | FLASH_ACR_PRFTEN;
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_7WS)
    {
    }

    /* 8. Configura Prescaler dei Bus: AHB=/1 (216MHz), APB1=/4 (54MHz), APB2=/2 (108MHz) */
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2);

    /* 9. Seleziona PLL come sorgente di SYSCLK */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
    }

    /* 10. Abilita I-Cache e D-Cache del Cortex-M7 */
    SCB_EnableICache();
    SCB_EnableDCache();
}
