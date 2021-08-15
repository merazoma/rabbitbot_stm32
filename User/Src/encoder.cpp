/*
 * encoder.cpp
 *
 *  Created on: May 28, 2021
 *      Author: masaya
 */
#include <encoder.h>
#include <simple_math.h>
#include "FreeRTOS.h"
#include "task.h"

#include "RabbitBotConfig.h"
#include "tim.h"

TIM_HandleTypeDef* htim_encoder[] = {&htim3, &htim1};
#define ResetCntConstValue 32768

Encoder::Encoder(motor_id_t id, uint16_t t_period, encoder_polarity_t polarity)
:htim(htim_encoder[id]), Tperiod(t_period), enc_pol(polarity), ResetCntValue(ResetCntConstValue), rotation_angle(0)
{}

Encoder::Encoder(const Encoder & encoder)
:htim(encoder.htim), Tperiod(encoder.Tperiod), enc_pol(encoder.enc_pol), ResetCntValue(ResetCntConstValue), rotation_angle(0)
{}

void Encoder::init()
{
	reset_count();
	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

float Encoder::get_velocity()
{
	velocity = nrps2velocity(nrps);
	return velocity;
}

void Encoder::count_periodic()
{
	calc_count_dif();
	calc_nrps();
	calc_rotation_angle();
}

int32_t Encoder::calc_count_dif()
{
	taskENTER_CRITICAL();
	enc_count = htim->Instance->CNT;
	reset_count();
	count_dif = (int32_t) ((enc_count - ResetCntValue) * enc_pol);
	taskEXIT_CRITICAL();
	return count_dif;
}

float Encoder::calc_nrps()
{
	nrps = 1000 * ((float)count_dif) / ((float)EncPPR * 4) / ((float)Tperiod) / ((float)GearRatio);
	return nrps;
}

float Encoder::calc_rotation_angle()
{
	rotation_angle += nrps * Tperiod * 2 * simple_math::PI;
//	rotation_angle = simple_math::get_mpi_pi(rotation_angle);
	return rotation_angle;
}

void Encoder::reset_count()
{
	htim->Instance->CNT = ResetCntValue;
}

float nrps2velocity(float nrps)
{
	return simple_math::PI * DiameterWheel * nrps;
}
