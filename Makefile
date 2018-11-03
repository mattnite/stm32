CC=arm-none-eabi-gcc
CFLAGS= -c -mthumb -mcpu=cortex-m0plus -mlittle-endian -IDrivers/CMSIS/Include -IDrivers/CMSIS/Device/ST/STM32L0xx/Include -g -nostartfiles -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DSTM32L073xx -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP
LD=arm-none-eabi-ld
LDFLAGS=-T linker.ld

main: startup_stm32l073xx.o src/main.o src/system_init.o
	$(LD) $(LDFLAGS) startup_stm32l073xx.o src/main.o src/system_init.o

startup_stm32l073xx.o:
	$(CC) $(CFLAGS) src/startup_stm32l073xx.s

 
