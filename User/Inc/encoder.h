/*
 * encoder.h
 *
 *  Created on: May 28, 2021
 *      Author: masaya
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <drive_motor.h>
#include "tim.h"

typedef int8_t encoder_polarity_t;
const encoder_polarity_t CLOCKWISE = 1;
const encoder_polarity_t ANTI_CLOCKWISE = -1;

class Encoder
{
private:
	TIM_HandleTypeDef* htim;
	uint16_t Tperiod;
	encoder_polarity_t enc_pol;
	int32_t count_dif;
	uint32_t enc_count;
	uint32_t ResetCntValue;
	int32_t calc_count_dif();
	float calc_rotation_angle();
	float calc_nrps();
public:
	float rotation_angle;
	float velocity;
	float nrps;
	Encoder(motor_id_t id, uint16_t Tperiod, encoder_polarity_t polarity);
	Encoder(const Encoder& encoder);
	void init();
	void reset_count();
	/**
	 * @brief Get the velocity object
	 *
	 * @return float タイヤの現在速度 [m/s]
	 */
	float get_velocity();
	void count_periodic();
};

/**
 * @brief 
 * 
 * @param nrps モータの回転数 [/s]
 * @return float タイヤの速度 [m/s]
 */
float nrps2velocity(float nrps);


#endif /* INC_ENCODER_H_ */
