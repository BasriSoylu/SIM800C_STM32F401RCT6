################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SIM800C_Lib/SIM800C.c \
../Drivers/SIM800C_Lib/sim800c_adapter.c 

OBJS += \
./Drivers/SIM800C_Lib/SIM800C.o \
./Drivers/SIM800C_Lib/sim800c_adapter.o 

C_DEPS += \
./Drivers/SIM800C_Lib/SIM800C.d \
./Drivers/SIM800C_Lib/sim800c_adapter.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SIM800C_Lib/%.o Drivers/SIM800C_Lib/%.su Drivers/SIM800C_Lib/%.cyclo: ../Drivers/SIM800C_Lib/%.c Drivers/SIM800C_Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I"C:/Users/SOYLU/Desktop/YAZILIM/STM/STM32_Workspaces/Bold_Motorbikes_Stm_Workspace/SIM800C_STM32F401RCT6/Drivers/SIM800C_Lib" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SIM800C_Lib

clean-Drivers-2f-SIM800C_Lib:
	-$(RM) ./Drivers/SIM800C_Lib/SIM800C.cyclo ./Drivers/SIM800C_Lib/SIM800C.d ./Drivers/SIM800C_Lib/SIM800C.o ./Drivers/SIM800C_Lib/SIM800C.su ./Drivers/SIM800C_Lib/sim800c_adapter.cyclo ./Drivers/SIM800C_Lib/sim800c_adapter.d ./Drivers/SIM800C_Lib/sim800c_adapter.o ./Drivers/SIM800C_Lib/sim800c_adapter.su

.PHONY: clean-Drivers-2f-SIM800C_Lib

