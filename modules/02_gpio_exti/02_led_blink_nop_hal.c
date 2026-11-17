/*
 * @file    02_led_blink_nop_hal.c
 * @brief   Blink del LED verde LD1 (PB0) con GPIO inizializzato via HAL e delay NOP.
 */

#include "stm32f7xx_hal.h"

static void delay_nop(uint32_t count)
{
    while (count > 0UL)
    {
        // Impedisce al compilatore di ottimizzare il ciclo vuoto
        __asm__ volatile("nop");
        count--;
    }
}

static void gpio_led_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    // Abilita il clock per GPIOB sul bus AHB1
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configura PB0 in uscita Push-Pull a bassa velocità
    gpio_init.Pin = GPIO_PIN_0;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &gpio_init);
}

int main(void)
{
    HAL_Init();
    gpio_led_init();

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        delay_nop(1000000UL);
    }
}
