/*
 * fb_control_motor.h
 *
 *  Created on: May 29, 2021
 *      Author: masaya
 */

#ifndef INC_FB_CONTROL_MOTOR_H_
#define INC_FB_CONTROL_MOTOR_H_

#include <drive_motor.h>
#include <encoder.h>

typedef struct {
	float x;
	float y;
	float theta;
} odom_pos_t;

typedef struct {
	float lin;
	float ang;
} odom_vel_t;

typedef struct {
	odom_pos_t position;
	odom_vel_t velocity;
	uint32_t time_stamp;
} odom_t;

typedef int8_t robot_direction_t;
const robot_direction_t ROBOT_FORWARD = 1;
const robot_direction_t ROBOT_BACKFORWARD = -1;

class FeedbackMotorController
{
private:
	float target_velocity;
	float old_velocity;
	float i_term;
	float d_term;
	float Pgain;
	float Igain;
	float Dgain;
	int16_t MaxIterm = 1000;

	// for data sampling
	float motor_duty;
	float velocity_now = 0;

public:
	MotorDriver* driver;
	Encoder* encoder;
	FeedbackMotorController(motor_id_t id, uint16_t t_period, encoder_polarity_t polarity);
	void init();
	void reset();
	/**
	 * @brief Set the target velocity object
	 * 
	 * @param velocity タイヤの目標並進速度 [m/s]
	 */
	void set_target_velocity(float velocity);
	void fb_control_periodic();

};

class DifferentialDriveController
{
private:
	FeedbackMotorController* fb_controller_left;
	FeedbackMotorController* fb_controller_right;
	//トレッド幅 unit: m
	//for sampling
	float target_lin_vel = 0;
	float target_ang_vel = 0;
	float wheel_velocity_cmd[2] = {0, 0};
	uint16_t step_time;

	void odom_calc_periodic();
	void fb_motor_control_periodic();
	void reset_odom();
public:
	odom_t odom;

	DifferentialDriveController(uint16_t interval_time, robot_direction_t direction);
	void init();
	void reset();
	/**
	 * @brief Set the cmd vel object
	 * 
	 * @param cmd_lin_vel 目標並進速度 [m/s]
	 * @param cmd_ang_vel 目標回転速度 [rad/s]
	 */
	void set_cmd_vel(float cmd_lin_vel, float cmd_ang_vel);
	void get_wheel_angles(double * wheel_angles);
	void get_wheel_angle_velocities(double * wheel_angle_velocities);
	void control_periodic();
};

#endif /* INC_FB_CONTROL_MOTOR_H_ */
