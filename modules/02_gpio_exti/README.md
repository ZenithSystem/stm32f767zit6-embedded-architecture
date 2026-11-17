# Modulo 02 — GPIO & EXTI (External Interrupts)

## Analisi dei Registri GPIO (RM0410 §7.4)
Ogni porta GPIO (`GPIOA` .. `GPIOK`) dispone di 16 pin ed è mappata sul bus **AHB1** (frequenza massima 216 MHz).

| Registro | Offset | Bit per Pin | Funzione |
| :--- | :--- | :--- | :--- |
| `MODER` | `0x00` | 2 bit | `00`: Input, `01`: General Purpose Output, `10`: Alternate Function, `11`: Analog |
| `OTYPER` | `0x04` | 1 bit | `0`: Push-Pull, `1`: Open-Drain |
| `OSPEEDR` | `0x08` | 2 bit | Slew rate (`00`: Low, `01`: Medium, `10`: High, `11`: Very High) |
| `PUPDR` | `0x0C` | 2 bit | `00`: No pull, `01`: Pull-up, `10`: Pull-down |
| `IDR` | `0x10` | 1 bit | Input Data Register (sola lettura) |
| `ODR` | `0x14` | 1 bit | Output Data Register (non atomico: soggetto a race condition con ISR!) |
| `BSRR` | `0x18` | 2 bit | **Bit Set/Reset Register** (scrittura atomica in singolo ciclo, immune a ISR) |
| `AFRL/H` | `0x20/24` | 4 bit | Selezione Alternate Function (`AF0` .. `AF15`) |
