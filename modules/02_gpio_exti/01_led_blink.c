/*
 * @file    01_led_blink.c
 * @brief   Blink del LED verde LD1 (PB0) su NUCLEO-F767ZI tramite puntatori volatile.
 */

#include <stdint.h>

// Indirizzi base delle periferiche (RM0410, Section 2.2.2, Table 1)
#define RCC_BASE   (0x40023800UL)
#define GPIOB_BASE (0x40020400UL)

// Registri memory-mapped con qualifica volatile
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE + 0x00UL))
#define GPIOB_ODR   (*(volatile uint32_t *)(GPIOB_BASE + 0x14UL))

// Maschere bit per clock GPIOB e pin PB0 (LD1)
#define RCC_GPIOB_EN (1UL << 1U)
#define PIN_0        (0U)

static void delay(volatile uint32_t count)
{
    while (count > 0UL)
    {
        count--;
    }
}

int main(void)
{
    // 1. Abilita il clock della porta GPIOB sul bus AHB1 (bit 1 = 1)
    RCC_AHB1ENR |= RCC_GPIOB_EN;

    // 2. Configura PB0 come General Purpose Output (MODER[1:0] = 0b01)
    GPIOB_MODER &= ~(3UL << (PIN_0 * 2U));
    GPIOB_MODER |= (1UL << (PIN_0 * 2U));

    while (1)
    {
        // Inverte lo stato logico del pin PB0 tramite XOR sull'Output Data Register
        GPIOB_ODR ^= (1UL << PIN_0);
        delay(1000000UL);
    }
}
