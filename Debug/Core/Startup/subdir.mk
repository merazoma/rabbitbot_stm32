################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f303k8tx.s 

S_DEPS += \
./Core/Startup/startup_stm32f303k8tx.d 

OBJS += \
./Core/Startup/startup_stm32f303k8tx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32f303k8tx.o: ../Core/Startup/startup_stm32f303k8tx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares" -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/User/Inc" -I"/home/masaya/STM32CubeIDE/workspace_1.6.0/rtos_f303/Middlewares/Third_Party/FreeRTOS" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32f303k8tx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

