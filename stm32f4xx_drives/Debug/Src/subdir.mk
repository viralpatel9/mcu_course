################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/013i2c_slave_tx_string.c 

OBJS += \
./Src/013i2c_slave_tx_string.o 

C_DEPS += \
./Src/013i2c_slave_tx_string.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Mcu_course github/mcu_course/stm32f4xx_drives/drivers/inc" -I"C:/Mcu_course github/mcu_course/stm32f4xx_drives/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/013i2c_slave_tx_string.d ./Src/013i2c_slave_tx_string.o ./Src/013i2c_slave_tx_string.su

.PHONY: clean-Src

