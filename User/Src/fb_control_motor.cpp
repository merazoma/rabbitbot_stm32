/*
 * fb_control_motor.cpp
 *
 *  Created on: May 29, 2021
 *      Author: masaya
 */
#include <fb_control_motor.h>
#include <drive_motor.h>
#include <encoder.h>
#include <rosserial_lib/ros.h>

#include "RabbitBotConfig.h"
#include "simple_math.h"
//#define ARM_MATH_CM4
//#include "arm_math.h"
//#include "arm_const_structs.h"

extern ros::NodeHandle nh;

const float MaxVelocity = nrps2velocity(MaxMotorNrps);
const float Velocity2Duty = 100/MaxVelocity;

FeedbackMotorController::FeedbackMotorController(motor_id_t id, uint16_t t_period, encoder_polarity_t polarity)
:Pgain(PgainMotor), Igain(IgainMotor), Dgain(DgainMotor)
{
	driver = new MotorDriver(id, polarity);
	encoder = new Encoder(id, t_period, polarity);
}

void FeedbackMotorController::init()
{
	driver->init();
	encoder->init();
	reset();
}

void FeedbackMotorController::reset()
{
	target_velocity = 0;
	old_velocity = 0;
	i_term = 0;
	d_term = 0;
	encoder->reset_count();
}

void FeedbackMotorController::set_target_velocity(float velocity)
{
	if (velocity < -MaxVelocity) {
		velocity = -MaxVelocity;
	} else if (velocity > MaxVelocity) {
		velocity = MaxVelocity;
	}
	target_velocity = velocity;
}

void FeedbackMotorController::fb_control_periodic()
{
	float dif_velocity;
	float err_sig;
	encoder->count_periodic();
	velocity_now = encoder->get_velocity();
	d_term = velocity_now - old_velocity;
	old_velocity = velocity_now;
	dif_velocity = target_velocity - velocity_now;
	err_sig = Velocity2Duty * dif_velocity;
	i_term += err_sig;
	if (i_term > MaxIterm) {
		i_term = MaxIterm;
	} else if (i_term < -MaxIterm) {
		i_term = -MaxIterm;
	}
	motor_duty = Pgain * err_sig + Igain * i_term + Dgain * d_term;
	if (motor_duty < 0) {
		driver->drive_motor_duty(-motor_duty, BACKWARD);
	} else {
		driver->drive_motor_duty(motor_duty, FORWARD);
	}
}


DifferentialDriveController::DifferentialDriveController(uint16_t interval_time, robot_direction_t direction)
:step_time(interval_time)
{
	fb_controller_left = new FeedbackMotorController(LEFT, interval_time, direction);
	fb_controller_right = new FeedbackMotorController(RIGHT, interval_time, -direction);
}

void DifferentialDriveController::reset()
{
	fb_controller_left->reset();
	fb_controller_right->reset();
	reset_odom();
}

void DifferentialDriveController::init()
{
	fb_controller_left->init();
	fb_controller_right->init();
	reset_odom();
}

void DifferentialDriveController::reset_odom()
{
	odom.position = {0, 0, 0};
	odom.velocity = {0, 0};
}

void DifferentialDriveController::set_cmd_vel(float cmd_lin_vel, float cmd_ang_vel)
{
	target_lin_vel = cmd_lin_vel;
	target_ang_vel = cmd_ang_vel;
	wheel_velocity_cmd[LEFT]   = cmd_lin_vel - (cmd_ang_vel * WheelSeparation / 2);
	wheel_velocity_cmd[RIGHT]  = cmd_lin_vel + (cmd_ang_vel * WheelSeparation / 2);
	fb_controller_left->set_target_velocity(wheel_velocity_cmd[LEFT]);
	fb_controller_right->set_target_velocity(wheel_velocity_cmd[RIGHT]);
}

void DifferentialDriveController::control_periodic()
{
	fb_motor_control_periodic();
	odom_calc_periodic();
}

void DifferentialDriveController::fb_motor_control_periodic()
{
	fb_controller_left->fb_control_periodic();
	fb_controller_right->fb_control_periodic();
}

void DifferentialDriveController::odom_calc_periodic()
{
	float vel_left_wheel = 0;
	float vel_right_wheel = 0;
//	float turning_r;
	vel_left_wheel = fb_controller_left->encoder->get_velocity();
	vel_right_wheel = fb_controller_right->encoder->get_velocity();
	odom.velocity.lin = (vel_left_wheel + vel_right_wheel)/2;
	odom.velocity.ang = (vel_right_wheel - vel_left_wheel)/WheelSeparation;
	odom.position.x += odom.velocity.lin * simple_math::cos(odom.position.theta + (odom.velocity.ang/2) * step_time/1000) * step_time/1000;
	odom.position.y += odom.velocity.lin * simple_math::sin(odom.position.theta + (odom.velocity.ang/2) * step_time/1000) * step_time/1000;
	odom.position.theta += odom.velocity.ang * step_time / 1000;
	odom.time_stamp = (uint32_t) xTaskGetTickCount();
	//	turning_r = odom.velocity.lin/odom.velocity.ang;
	//	odom.position.x += -turning_r * sin(odom.position.theta) + turning_r * sin(odom.position.theta + odom.velocity.ang * step_time/1000);
	//	odom.position.y += turning_r * cos(odom.position.theta) - turning_r * cos(odom.position.theta + odom.velocity.ang * step_time/1000);
	//	odom.position.theta += odom.velocity.ang * step_time / 1000;
}

void DifferentialDriveController::get_wheel_angles(double * wheel_angles)
{
	wheel_angles[0] = (double)fb_controller_left->encoder->rotation_angle;
	wheel_angles[1] = (double)fb_controller_right->encoder->rotation_angle;
}

void DifferentialDriveController::get_wheel_angle_velocities(double * wheel_angle_velocities)
{
	wheel_angle_velocities[0] = fb_controller_left->encoder->nrps * 2 * simple_math::PI;
	wheel_angle_velocities[1] = fb_controller_right->encoder->nrps * 2 * simple_math::PI;
}
