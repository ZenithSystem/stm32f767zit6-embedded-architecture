/*
 * @file    main.c
 * @brief   Blink sequenziale dei LED utente (PB0, PB7, PB14) su NUCLEO-F767ZI.
 *
 * Al reset il core esegue sull'oscillatore interno HSI a 16 MHz.
 * SysTick genera un tick hardware ogni 1 ms senza utilizzo di interrupt.
 */

#include "stm32f767_regs.h"

// Pin LED sulla NUCLEO-F767ZI (porta GPIOB, logica attiva alta)
#define LED_GREEN_PIN (0U)  // LD1 -> PB0
#define LED_BLUE_PIN  (7U)  // LD2 -> PB7
#define LED_RED_PIN   (14U) // LD3 -> PB14

#define HSI_CLOCK_HZ (16000000UL)

static void systick_init(void)
{
    // Reload = (16,000,000 / 1,000) - 1 = 15,999 cicli -> periodo 1 ms
    SysTick->LOAD = (HSI_CLOCK_HZ / 1000UL) - 1UL;
    SysTick->VAL = 0UL;
    SysTick->CTRL = SYST_CSR_CLKSOURCE | SYST_CSR_ENABLE;
}

static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0U; i < ms; i++)
    {
        // COUNTFLAG va a 1 al raggiungimento dello 0 e si azzera alla lettura
        while ((SysTick->CTRL & SYST_CSR_COUNTFLAG) == 0UL)
        {
        }
    }
}

static void gpio_leds_init(void)
{
    // 1. Abilita clock per GPIOB sul bus AHB1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Read-back per attendere la stabilizzazione del clock (RM0410 §5.2.12)
    (void)RCC->AHB1ENR;

    // 2. MODER = 0b01 (General purpose output mode) su PB0, PB7, PB14
    GPIOB->MODER &= ~((3UL << (LED_GREEN_PIN * 2U)) | (3UL << (LED_BLUE_PIN * 2U)) |
                      (3UL << (LED_RED_PIN * 2U)));

    GPIOB->MODER |= ((1UL << (LED_GREEN_PIN * 2U)) | (1UL << (LED_BLUE_PIN * 2U)) |
                     (1UL << (LED_RED_PIN * 2U)));

    // 3. OTYPER = 0 (Output push-pull)
    GPIOB->OTYPER &= ~((1UL << LED_GREEN_PIN) | (1UL << LED_BLUE_PIN) | (1UL << LED_RED_PIN));

    // 4. OSPEEDR = 0b00 (Low speed)
    GPIOB->OSPEEDR &= ~((3UL << (LED_GREEN_PIN * 2U)) | (3UL << (LED_BLUE_PIN * 2U)) |
                        (3UL << (LED_RED_PIN * 2U)));

    // 5. PUPDR = 0b00 (No pull-up, no pull-down)
    GPIOB->PUPDR &= ~((3UL << (LED_GREEN_PIN * 2U)) | (3UL << (LED_BLUE_PIN * 2U)) |
                      (3UL << (LED_RED_PIN * 2U)));
}

static inline void led_on(uint32_t pin)
{
    // Bit [15:0] di BSRR -> Set atomico del pin
    GPIOB->BSRR = (1UL << pin);
}

static inline void led_off(uint32_t pin)
{
    // Bit di BSRR -> Reset atomico del pin
    GPIOB->BSRR = (1UL << (pin + 16U));
}

int main(void)
{
    systick_init();
    gpio_leds_init();

    while (1)
    {
        led_on(LED_GREEN_PIN);
        delay_ms(250U);
        led_off(LED_GREEN_PIN);

        led_on(LED_BLUE_PIN);
        delay_ms(250U);
        led_off(LED_BLUE_PIN);

        led_on(LED_RED_PIN);
        delay_ms(250U);
        led_off(LED_RED_PIN);
    }
}
