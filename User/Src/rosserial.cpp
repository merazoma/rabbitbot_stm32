/*
 * rosserial.cpp
 *
 *  Created on: May 29, 2021
 *      Author: masaya
 */

#include <rosserial.h>
#include <rosserial_lib/ros.h>
#include <rosserial_lib/std_msgs/Float32.h>
//#include <rosserial_lib/std_msgs/String.h>
//#include <rosserial_lib/std_msgs/Header.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Pose2D.h>
#include <rosserial_lib/geometry_msgs/Twist.h>
#include <rosserial_lib/geometry_msgs/TwistStamped.h>
#include <sensor_msgs/BatteryState.h>
//#include <rosserial_lib/nav_msgs/Odometry.h>

#include "RabbitBotConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "simple_math.h"
#include <fb_control_motor.h>
#include <battery_monitor.h>

/* ROS */
ros::NodeHandle nh;
/* Publisher */

geometry_msgs::Pose2D odom_pose2D;
ros::Publisher odom_pose_pub("odom_pose2D", &odom_pose2D);
geometry_msgs::TwistStamped odom_velocity;
ros::Publisher odom_velocity_pub("odom_velocity", &odom_velocity);

sensor_msgs::JointState joint_states;
ros::Publisher joint_states_pub("joint_states", &joint_states);

//sensor_msgs::BatteryState battery_state_msg;
//ros::Publisher battery_msg_pub("battery_msg", &battery_state_msg);
std_msgs::Float32 bat_voltage;
ros::Publisher battery_vol_pub("battery_voltage", &bat_voltage);

/* Subscriber */
void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg);
ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", commandVelocityCallback);

void resetCallback(const std_msgs::Empty& reset_msg);
ros::Subscriber<std_msgs::Empty> reset_sub("reset", resetCallback);

/* FreeRTOS */
void RosSerialTask(void * argument);
TaskHandle_t rosserialTaskHandle;

/* Timer */
TimerHandle_t repeatRosserialHandle;
void repeatRosserialCallBack(TimerHandle_t xTimer);


extern DifferentialDriveController dif_drive_controller;
odom_t odom;
extern BatteryMonitor battery_monitor;
battery_state_t battery_state;

void initJointStates(void);
void publishOdom(void);
void publishJointStates(void);
void publishBatteryMsg(void);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
//	SEGGER_SYSVIEW_RecordEnterISR();
//	UBaseType_t uxSavedInterruptStatus;
//	taskENTER_CRITICAL_FROM_ISR();
	nh.getHardware()->flush();
//	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
//	SEGGER_SYSVIEW_RecordExitISR();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//	SEGGER_SYSVIEW_RecordEnterISR();
//	UBaseType_t uxSavedInterruptStatus;
//	taskENTER_CRITICAL_FROM_ISR();
	nh.getHardware()->reset_rbuf();
//	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
//	SEGGER_SYSVIEW_RecordExitISR();
}

void commandVelocityCallback(const geometry_msgs::Twist& cmd_vel_msg)
{
	float cmd_lin_vel = 0;
	float cmd_ang_vel = 0;

//	SEGGER_SYSVIEW_PrintfHost("Subscribe cmd_vel");
	cmd_lin_vel = cmd_vel_msg.linear.x;
	cmd_ang_vel = cmd_vel_msg.angular.z;
	dif_drive_controller.set_cmd_vel(cmd_lin_vel, cmd_ang_vel);
}

void resetCallback(const std_msgs::Empty& reset_msg)
{
	static uint16_t cnt_reset = 0;
	cnt_reset ++;
	dif_drive_controller.reset();

}

void init_rosserial(void)
{
	if (xTaskCreate(RosSerialTask, "RosSerialTask", 256, NULL, tskIDLE_PRIORITY+3, &rosserialTaskHandle) != pdPASS)
	{
		while(1);
	}
	repeatRosserialHandle = xTimerCreate("RosserialRepeatTimer", TPauseRosSerial/portTICK_PERIOD_MS, pdTRUE, NULL, repeatRosserialCallBack);
	assert_param(repeatRosserialHandle != NULL);
	xTimerStart(repeatRosserialHandle, 0);

	//	if (xTaskCreate(StringRosPubTask, "StringRosPubTask", 64, NULL, tskIDLE_PRIORITY+2, NULL) != pdPASS)
//	{
//		while(1);
//	}
//	if (xTaskCreate(StringClientTask1, "StringClientTask1", 64, NULL, tskIDLE_PRIORITY+4, NULL) != pdPASS)
//	{
//		while(1);
//	}
//	odomQueue = xQueueCreate(3, sizeof(odom_t));
//	assert_param(odomQueue != NULL);
}

void RosSerialTask(void * argument)
{
	static uint16_t cnt = 0;

	nh.initNode();
	nh.subscribe(cmd_vel_sub);
	nh.subscribe(reset_sub);

//	nh.advertise(odom_header_pub);
	nh.advertise(odom_pose_pub);
	nh.advertise(odom_velocity_pub);
	nh.advertise(joint_states_pub);
//	nh.advertise(battery_msg_pub);
	nh.advertise(battery_vol_pub);
	initJointStates();
	nh.loginfo("stmCR node init done.");
	nh.spinOnce();

	for (;;)
	{
		if (ulTaskNotifyTake(pdFALSE, 2 * TPauseRosSerial / portTICK_PERIOD_MS) != 0)
		{
			publishOdom();
			// JointStatesは、DivisionPubJointStatesで分周した周期でPublish
			if (cnt % DivisionPubJointStates == 0)
			{
				publishJointStates();
			}
			if ((cnt+1) % DivisionBatteryMsg == 0)
			{
				publishBatteryMsg();
			}
			nh.spinOnce();
			cnt++;
		}
		else
		{
			while(1);
		}
	}

}

void publishOdom(void)
{
	odom = dif_drive_controller.odom;
	odom_pose2D.x = odom.position.x;
	odom_pose2D.y = odom.position.y;
	odom_pose2D.theta = odom.position.theta;
	odom_velocity.header.stamp = nh.get_ros_time(odom.time_stamp);
	odom_velocity.header.frame_id = OdomHeaderFrameID;
	odom_velocity.twist.linear.x = odom.velocity.lin;
	odom_velocity.twist.angular.z = odom.velocity.ang;
//	odom_header_pub.publish(&odom_header);
	odom_pose_pub.publish(&odom_pose2D);
	odom_velocity_pub.publish(&odom_velocity);
}

void publishJointStates(void)
{
	static double joint_states_pos[WHEEL_NUM] = {0.0, 0.0};
	static double joint_states_vel[WHEEL_NUM] = {0.0, 0.0};
	joint_states.header.stamp = nh.get_ros_time(odom.time_stamp);
	dif_drive_controller.get_wheel_angles(joint_states_pos);
	dif_drive_controller.get_wheel_angle_velocities(joint_states_vel);
	joint_states.position = joint_states_pos;
	joint_states.velocity = joint_states_vel;
	joint_states_pub.publish(&joint_states);
}

void publishBatteryMsg(void)
{
	bat_voltage.data = battery_monitor.battery_voltage;
	battery_vol_pub.publish(&bat_voltage);
}

//void publishBatteryMsg(void)
//{
//	battery_state = battery_monitor.get_battery_state();
//	battery_state_msg.header.stamp = nh.get_ros_time(battery_state.time_stamp);
//	battery_state_msg.capacity = battery_state.capacity;
//	battery_state_msg.voltage = battery_state.voltage;
//	battery_state_msg.percentage = battery_state.percentage;
//	battery_msg_pub.publish(&battery_state_msg);
//}

void initJointStates(void)
{
	static char *joint_states_name[] = {(char*)LeftWheelJointStateName, (char*)RightWheelJointStateName};

	joint_states.header.frame_id = JointStateFrameID;
	joint_states.name            = joint_states_name;
	joint_states.name_length     = WHEEL_NUM;
	joint_states.position_length = WHEEL_NUM;
	joint_states.velocity_length = WHEEL_NUM;
	joint_states.effort_length   = WHEEL_NUM;
}

void repeatRosserialCallBack(TimerHandle_t xTimer)
{
	xTaskNotifyGive(rosserialTaskHandle);
}
