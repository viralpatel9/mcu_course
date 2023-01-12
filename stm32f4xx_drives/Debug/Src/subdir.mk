################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/012i2c_master_rx_testingIT.c 

OBJS += \
./Src/012i2c_master_rx_testingIT.o 

C_DEPS += \
./Src/012i2c_master_rx_testingIT.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Mcu_course github/mcu_course/stm32f4xx_drives/drivers/inc" -I"C:/Mcu_course github/mcu_course/stm32f4xx_drives/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/012i2c_master_rx_testingIT.d ./Src/012i2c_master_rx_testingIT.o ./Src/012i2c_master_rx_testingIT.su

.PHONY: clean-Src

