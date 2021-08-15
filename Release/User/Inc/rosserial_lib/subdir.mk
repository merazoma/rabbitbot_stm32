################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../User/Inc/rosserial_lib/duration.cpp \
../User/Inc/rosserial_lib/time.cpp 

OBJS += \
./User/Inc/rosserial_lib/duration.o \
./User/Inc/rosserial_lib/time.o 

CPP_DEPS += \
./User/Inc/rosserial_lib/duration.d \
./User/Inc/rosserial_lib/time.d 


# Each subdirectory must supply rules for building sources it contributes
User/Inc/rosserial_lib/duration.o: ../User/Inc/rosserial_lib/duration.cpp User/Inc/rosserial_lib/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares" -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares/Third_Party/FreeRTOS" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"User/Inc/rosserial_lib/duration.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
User/Inc/rosserial_lib/time.o: ../User/Inc/rosserial_lib/time.cpp User/Inc/rosserial_lib/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F303x8 -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares" -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares/Third_Party/FreeRTOS" -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"User/Inc/rosserial_lib/time.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

