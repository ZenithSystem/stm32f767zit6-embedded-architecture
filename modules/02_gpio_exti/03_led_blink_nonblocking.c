/*
 * @file    03_led_blink_nonblocking.c
 * @brief   Blink non-bloccante del LED verde LD1 (PB0) tramite SysTick hardware (1 ms).
 */

#include <stdint.h>

// Indirizzi base delle periferiche STM32F767 (RM0410, Section 2.2.2)
#define RCC_BASE   (0x40023800UL)
#define GPIOB_BASE (0x40020400UL)

// Indirizzo base SysTick nel System Control Space del Cortex-M7
#define SYSTICK_BASE (0xE000E010UL)

// Registri RCC e GPIOB
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_BASE + 0x00UL))
#define GPIOB_ODR   (*(volatile uint32_t *)(GPIOB_BASE + 0x14UL))

// Registri SysTick
#define SYST_CSR (*(volatile uint32_t *)(SYSTICK_BASE + 0x00UL))
#define SYST_RVR (*(volatile uint32_t *)(SYSTICK_BASE + 0x04UL))
#define SYST_CVR (*(volatile uint32_t *)(SYSTICK_BASE + 0x08UL))

// Maschere bit per RCC, GPIO e SysTick
#define RCC_GPIOB_EN (1UL << 1U)
#define PIN_0        (0U)

#define SYST_CSR_ENABLE    (1UL << 0U)
#define SYST_CSR_CLKSOURCE (1UL << 2U)
#define SYST_CSR_COUNTFLAG (1UL << 16U)

#define HSI_CLOCK_HZ    (16000000UL)
#define BLINK_PERIOD_MS (500UL)

static void systick_init(void)
{
    // Imposta periodo di 1 ms con clock HSI a 16 MHz (16000 cicli -> reload = 15999)
    SYST_RVR = (HSI_CLOCK_HZ / 1000UL) - 1UL;

    // Azzera il contatore corrente e il flag COUNTFLAG
    SYST_CVR = 0UL;

    // Abilita SysTick usando il clock del processore (senza interrupt)
    SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_ENABLE;
}

int main(void)
{
    uint32_t ms_ticks = 0UL;
    uint32_t last_toggle_ms = 0UL;

    // 1. Abilita il clock della porta GPIOB sul bus AHB1
    RCC_AHB1ENR |= RCC_GPIOB_EN;

    // 2. Configura PB0 come General Purpose Output (MODER[1:0] = 0b01)
    GPIOB_MODER &= ~(3UL << (PIN_0 * 2U));
    GPIOB_MODER |= (1UL << (PIN_0 * 2U));

    // 3. Avvia il timer hardware SysTick con base tempi a 1 ms
    systick_init();

    while (1)
    {
        // Verifica se è trascorso 1 ms (la lettura di SYST_CSR azzera COUNTFLAG)
        if ((SYST_CSR & SYST_CSR_COUNTFLAG) != 0UL)
        {
            ms_ticks++;
        }

        // Commuta il LED ogni 500 ms senza mai bloccare l'esecuzione del loop
        if ((ms_ticks - last_toggle_ms) >= BLINK_PERIOD_MS)
        {
            last_toggle_ms = ms_ticks;
            GPIOB_ODR ^= (1UL << PIN_0);
        }

        // Qui la CPU è libera di eseguire altri task senza attese bloccanti
    }
}
