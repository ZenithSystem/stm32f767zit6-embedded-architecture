/*
 * @file    03_led_blink_nonblocking_hal.c
 * @brief   Blink non-bloccante del LED verde LD1 (PB0) tramite HAL_GetTick().
 */

#include "stm32f7xx_hal.h"

#define BLINK_PERIOD_MS (500UL)

void SysTick_Handler(void)
{
    // Incrementa il tick hardware della HAL ogni 1 ms
    HAL_IncTick();
}

static void gpio_led_init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    // Abilita il clock per GPIOB sul bus AHB1
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configura PB0 come General Purpose Output Push-Pull
    gpio_init.Pin = GPIO_PIN_0;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &gpio_init);
}

int main(void)
{
    uint32_t last_toggle_ms = 0UL;

    HAL_Init();
    gpio_led_init();

    while (1)
    {
        // Confronta il tempo trascorso senza bloccare l'esecuzione del loop
        if ((HAL_GetTick() - last_toggle_ms) >= BLINK_PERIOD_MS)
        {
            last_toggle_ms = HAL_GetTick();
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        }
    }
}
