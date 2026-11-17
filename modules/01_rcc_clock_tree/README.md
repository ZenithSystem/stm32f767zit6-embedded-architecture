# Modulo 01 — RCC & Clock Tree Configuration (216 MHz)

## Configurazione
Portare il system clock (`SYSCLK`) dell'STM32F767ZIT6 alla massima frequenza operativa di **216 MHz** partendo dall'oscillatore esterno da **8 MHz** fornito dall'ST-LINK su NUCLEO-F767ZI (`HSE Bypass`), configurando correttamente regolatore di tensione, Over-Drive Mode e Wait States della memoria Flash.

## Calcolo dei Divisori e Moltiplicatori PLL (RM0410 §5.3)

$$f_{VCO\_in} = \frac{f_{HSE}}{PLLM} = \frac{8\text{ MHz}}{4} = 2\text{ MHz} \quad (\text{Range consigliato: } 1 - 2\text{ MHz})$$

$$f_{VCO\_out} = f_{VCO\_in} \times PLLN = 2\text{ MHz} \times 216 = 432\text{ MHz}$$

$$f_{SYSCLK} = \frac{f_{VCO\_out}}{PLLP} = \frac{432\text{ MHz}}{2} = 216\text{ MHz}$$

$$f_{USB/RNG/SDMMC} = \frac{f_{VCO\_out}}{PLLQ} = \frac{432\text{ MHz}}{9} = 48\text{ MHz}$$

## Prescaler dei Bus Periferici
* **AHB (`HCLK`)**: `SYSCLK / 1` = **216 MHz**
* **APB1 (`PCLK1`)**: `HCLK / 4` = **54 MHz** (Limite massimo datasheet: 54 MHz; Timer APB1 clock = 108 MHz)
* **APB2 (`PCLK2`)**: `HCLK / 2` = **108 MHz** (Limite massimo datasheet: 108 MHz; Timer APB2 clock = 216 MHz)
