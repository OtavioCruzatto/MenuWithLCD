################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Misc/src/lcdController.c \
../Misc/src/pushButton.c 

OBJS += \
./Misc/src/lcdController.o \
./Misc/src/pushButton.o 

C_DEPS += \
./Misc/src/lcdController.d \
./Misc/src/pushButton.d 


# Each subdirectory must supply rules for building sources it contributes
Misc/src/%.o Misc/src/%.su: ../Misc/src/%.c Misc/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Otavi/Documents/Embed/STM32/Projects/MenuWithLCD/Misc/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Misc-2f-src

clean-Misc-2f-src:
	-$(RM) ./Misc/src/lcdController.d ./Misc/src/lcdController.o ./Misc/src/lcdController.su ./Misc/src/pushButton.d ./Misc/src/pushButton.o ./Misc/src/pushButton.su

.PHONY: clean-Misc-2f-src

