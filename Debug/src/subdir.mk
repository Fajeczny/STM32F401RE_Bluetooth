################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32 -DSTM32F4 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F401xx -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/inc" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/CMSIS/device" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/CMSIS/core" -I"/home/fajek/Documents/Eclipse/STM32F401RE_BT/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


