/**
 * @file    gpio_led_baremetal.c
 * @brief   Driver Bare-Metal per LED utente (PB0, PB7, PB14) e Button EXTI13 (PC13).
 * @target  NUCLEO-F767ZI
 */

#include "stm32f767xx.h"

void GPIO_Nucleo_Init(void)
{
    /* 1. Abilita clock su GPIOB, GPIOC (AHB1) e SYSCFG (APB2, necessario per EXTI) */
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    (void)RCC->APB2ENR;

    /* 2. Configura PB0 (LD1), PB7 (LD2), PB14 (LD3) come Output Push-Pull Low Speed */
    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER7 | GPIO_MODER_MODER14);
    GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER14_0);

    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT7 | GPIO_OTYPER_OT14);
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0 | GPIO_OSPEEDR_OSPEEDR7 | GPIO_OSPEEDR_OSPEEDR14);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR14);

    /* 3. Configura PC13 (User Button B1) come Input Floating (pull-down esterno su board) */
    GPIOC->MODER &= ~GPIO_MODER_MODER13;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13;

    /* 4. Instrada PC13 verso la linea EXTI13 tramite SYSCFG_EXTICR4 */
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

    /* 5. Abilita Interrupt Mask e Rising Edge Trigger su EXTI13 */
    EXTI->IMR |= EXTI_IMR_IM13;
    EXTI->RTSR |= EXTI_RTSR_TR13;
    EXTI->FTSR &= ~EXTI_FTSR_TR13;

    /* 6. Abilita IRQ EXTI15_10 nel controller NVIC */
    NVIC_SetPriority(EXTI15_10_IRQn, 5U);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void LED_Set(uint32_t pin_mask)
{
    GPIOB->BSRR = pin_mask;
}

void LED_Clear(uint32_t pin_mask)
{
    GPIOB->BSRR = (pin_mask << 16U);
}

void EXTI15_10_IRQHandler(void)
{
    if ((EXTI->PR & EXTI_PR_PR13) != 0U)
    {
        EXTI->PR = EXTI_PR_PR13;

        if ((GPIOB->ODR & GPIO_ODR_OD7) != 0U)
        {
            LED_Clear(GPIO_BSRR_BS7);
        }
        else
        {
            LED_Set(GPIO_BSRR_BS7);
        }
    }
}
