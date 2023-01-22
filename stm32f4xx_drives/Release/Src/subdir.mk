################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/013i2c_slave_tx_string.c \
../Src/014i2c_slave_tx_string2.c \
../Src/015uart_tx.c \
../Src/016uart_case.c \
../Src/017rtc_lcd.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/013i2c_slave_tx_string.o \
./Src/014i2c_slave_tx_string2.o \
./Src/015uart_tx.o \
./Src/016uart_case.o \
./Src/017rtc_lcd.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/013i2c_slave_tx_string.d \
./Src/014i2c_slave_tx_string2.d \
./Src/015uart_tx.d \
./Src/016uart_case.d \
./Src/017rtc_lcd.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DRELEASE -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Mcu_course github/mcu_course/stm32f4xx_drives/drivers/inc" -O3 -ffunction-sections -fdata-sections -Wall -Wextra -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/013i2c_slave_tx_string.d ./Src/013i2c_slave_tx_string.o ./Src/013i2c_slave_tx_string.su ./Src/014i2c_slave_tx_string2.d ./Src/014i2c_slave_tx_string2.o ./Src/014i2c_slave_tx_string2.su ./Src/015uart_tx.d ./Src/015uart_tx.o ./Src/015uart_tx.su ./Src/016uart_case.d ./Src/016uart_case.o ./Src/016uart_case.su ./Src/017rtc_lcd.d ./Src/017rtc_lcd.o ./Src/017rtc_lcd.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

