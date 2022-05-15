################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MKL46Z4_BTN_XMODEM.c \
../source/Tiempo.c \
../source/buffer.c \
../source/button.c \
../source/display.c \
../source/mtb.c \
../source/pit.c \
../source/semihost_hardfault.c \
../source/showDisplay.c \
../source/uart_ringBufer.c \
../source/xmodem.c 

OBJS += \
./source/MKL46Z4_BTN_XMODEM.o \
./source/Tiempo.o \
./source/buffer.o \
./source/button.o \
./source/display.o \
./source/mtb.o \
./source/pit.o \
./source/semihost_hardfault.o \
./source/showDisplay.o \
./source/uart_ringBufer.o \
./source/xmodem.o 

C_DEPS += \
./source/MKL46Z4_BTN_XMODEM.d \
./source/Tiempo.d \
./source/buffer.d \
./source/button.d \
./source/display.d \
./source/mtb.d \
./source/pit.d \
./source/semihost_hardfault.d \
./source/showDisplay.d \
./source/uart_ringBufer.d \
./source/xmodem.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\board" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\source" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\drivers" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\utilities" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\startup" -I"C:\Users\Nicolas\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_BTN_XMODEM\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


