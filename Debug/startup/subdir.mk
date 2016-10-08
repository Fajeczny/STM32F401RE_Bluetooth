################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f401xx.s 

OBJS += \
./startup/startup_stm32f401xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/inc" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/CMSIS/device" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/CMSIS/core" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/StdPeriph_Driver/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


