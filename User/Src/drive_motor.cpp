/*
 * control_motor.cpp
 *
 *  Created on: May 24, 2021
 *      Author: masaya
 */
#include <drive_motor.h>
#include "tim.h"

static GPIO_TypeDef * IN1_GPIO_Port[2] = {IN1_LEFT_GPIO_Port, IN1_RIGHT_GPIO_Port};
static GPIO_TypeDef * IN2_GPIO_Port[2] = {IN2_LEFT_GPIO_Port, IN2_RIGHT_GPIO_Port};
static uint16_t IN1_Pin[2] = {IN1_LEFT_Pin, IN1_RIGHT_Pin};
static uint16_t IN2_Pin[2] = {IN2_LEFT_Pin, IN2_RIGHT_Pin};

static uint32_t TIM_CHANNEL[2] = {TIM_CHANNEL_2, TIM_CHANNEL_1};
static const uint32_t HTIMPeriod = 999;

MotorDriver::MotorDriver(motor_id_t id, motor_direction_pol_t direction)
: motor_id(id), motor_direction(direction)
{}

MotorDriver::MotorDriver(const MotorDriver& motor_driver)
: motor_id(motor_driver.motor_id)
{}

void MotorDriver::init()
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL[motor_id]);
	HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port, MOTOR_STBY_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port, MOTOR_STBY_Pin, GPIO_PIN_SET);
}

void MotorDriver::drive_motor_duty(float duty, motor_direction_t direction)
{
	switch (direction * motor_direction)
	{
		case FORWARD:
			HAL_GPIO_WritePin(IN1_GPIO_Port[motor_id], IN1_Pin[motor_id], GPIO_PIN_SET);
			HAL_GPIO_WritePin(IN2_GPIO_Port[motor_id], IN2_Pin[motor_id], GPIO_PIN_RESET);
			break;
		case BACKWARD:
			HAL_GPIO_WritePin(IN1_GPIO_Port[motor_id], IN1_Pin[motor_id], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN2_GPIO_Port[motor_id], IN2_Pin[motor_id], GPIO_PIN_SET);
		default:
			assert_param(1);
			break;
	}
	drive_pwm(duty);
}

/**
 * @brief 指定されたdutyでPWM出力
 * @details PWM frequency = 64 MHz / (Prescaler + 1) / (CounterPeriod + 1) = 64 MHz / (1 + 1) / (999 + 1) = 32 kHz
 * @param duty 0 ~ 100 [%]
 * 
 */
void MotorDriver::drive_pwm(float duty)
{
	if (duty < 0) {
		duty = 0;
	} else if (duty > 100) {
		duty = 100;
	}
	uint32_t pulse = (uint32_t)(HTIMPeriod * duty / 100);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL[motor_id], pulse);
}
