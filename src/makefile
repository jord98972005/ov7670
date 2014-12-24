EXECUTABLE=main.elf
BIN_IMAGE=main.bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
CFLAGS=-g -O0 -mlittle-endian -mthumb
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib
# to run from FLASH
STMLIB = ./inc
CFLAGS+=-Wl,-T,$(STMLIB)/stm32_flash.ld
CFLAGS+=-I./
# stm32f4_discovery lib
CFLAGS+=-I$(STMLIB)
CFLAGS+=-I$(STMLIB)/STM32F10x_StdPeriph_Driver/inc

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
        $(OBJCOPY) -O binary $^ $@
        arm-none-eabi-objdump -S main.elf > main.list

$(EXECUTABLE): \
        src/main.c \
        src/Sensor.c \
        src/stm32f10x_it.c \
        src/lcd.c \
        src/sccb.c \
        src/fifo.c\
        src/delay.c \
        #inc/system_stm32f4xx.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_nvic.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_dcmi.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c \
        inc/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c \
        inc/STM32F10x_StdPeriph_Driver/src/misc.c
        $(CC) $(CFLAGS) $^ -o $@  

run : main.bin gdbscript
        ./run.sh $<
clean:
        rm -rf $(EXECUTABLE)
        rm -rf $(BIN_IMAGE)
        rm -rf main.list
.PHONY: all clean

flash: $(BIN_IMAGE)
        st-flash write $< 0x8000000
        
gdbauto: $(BIN_IMAGE)
        arm-none-eabi-gdb -x gdbauto.sh