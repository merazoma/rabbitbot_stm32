/*
 * control_motor.hpp
 *
 *  Created on: May 24, 2021
 *      Author: masaya
 */

#ifndef INC_DRIVE_MOTOR_H_
#define INC_DRIVE_MOTOR_H_

#ifdef __cplusplus
 extern "C" {
#endif

//#include "main.h"

/**
 * @brief 左右モータ選択用の型。control_motor_Nrpmやdrive_motor_dutyの引数として使用。
 *
 */
typedef enum enm_motor_id{
    //! 左モータ
    LEFT = 0,
    //! 右モータ
    RIGHT
} motor_id_t;

/**
 * @brief モータの正転・逆転選択用の型。drive_motor_dutyの引数で使用。
 *
 */
typedef int motor_direction_pol_t;
const motor_direction_pol_t FORWARD_POL = 1;
const motor_direction_pol_t BACKWARD_POL = -1;

typedef int motor_direction_t;
const motor_direction_t FORWARD = 1;
const motor_direction_t BACKWARD = -1;
//typedef enum enm_motor_direction{
//    //! モータ正転
//    FORWARD = 0,
//    //! モータ逆転
//    BACKWARD
//} motor_direction_t;

class MotorDriver
{
private:
	motor_id_t motor_id;
	int motor_direction;
	void drive_pwm(float duty);
public:
	MotorDriver(motor_id_t id, motor_direction_pol_t direction);
	MotorDriver(const MotorDriver& motor_driver);
	void init();
	void drive_motor_duty(float duty, motor_direction_t direction);
};


#ifdef __cplusplus
}
#endif
#endif /* INC_DRIVE_MOTOR_H_ */
