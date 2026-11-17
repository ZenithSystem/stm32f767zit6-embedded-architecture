TARGET   = led_blink
CC       = arm-none-eabi-gcc
OBJCOPY  = arm-none-eabi-objcopy
SIZE     = arm-none-eabi-size

MCU      = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
CFLAGS   = $(MCU) -O2 -Wall -Wextra -std=c11 -ffunction-sections -fdata-sections
LDFLAGS  = $(MCU) -T STM32F767ZITx.ld -nostdlib -Wl,--gc-sections,-Map=$(TARGET).map

SRCS     = startup.c main.c
OBJS     = $(SRCS:.c=.o)

all: $(TARGET).bin $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

%.o: %.c stm32f767_regs.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

flash: $(TARGET).elf
	openocd -f ../../../../tools/openocd/stm32f7x.cfg -c "program $(TARGET).elf verify reset exit"

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).map

.PHONY: all flash clean
