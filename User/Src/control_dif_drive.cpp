/*
 * control_dif_drive.c
 *
 *  Created on: May 24, 2021
 *      Author: masaya
 */
#include <control_dif_drive.h>
#include <fb_control_motor.h>
//#include "rosserial.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "RabbitBotConfig.h"
#include "main.h"
#include "timers.h"
// #include <SEGGER_SYSVIEW.h>

extern QueueHandle_t odomQueue;
TaskHandle_t difDriveTaskHandle;
void difDriveTask(void * argument);
TimerHandle_t controllerRepeatTimerHandle;
void difDriveCallBack(TimerHandle_t xTimer);

TaskHandle_t driveInfoPubTaskHandle;
void driveInfoPubTask(void * argument);
TimerHandle_t driveInfoPubRepeatTimerHandle;
void driveInfoPubCallBack(TimerHandle_t xTimer);

DifferentialDriveController dif_drive_controller(DifDrivePeriod, ROBOT_FORWARD);

void test_sin();

void init_dif_drive()
{
	dif_drive_controller.init();
}

void init_dif_drive_task()
{
	/* Task Create */
	if (xTaskCreate(difDriveTask, "MotorFeedBackTask", 200, NULL, 4, &difDriveTaskHandle) != pdPASS)
	{
		while(1);
	}
//	if (xTaskCreate(driveInfoPubTask, "DriveInfoPublishTask", 128, NULL, 3, &driveInfoPubTaskHandle) != pdPASS)
//	{
//		while(1);
//	}

	/* Timer Create*/
	controllerRepeatTimerHandle = xTimerCreate("DifDriveControllerRepeatTimer", DifDrivePeriod/portTICK_PERIOD_MS, pdTRUE, NULL, difDriveCallBack);
	assert_param(controllerRepeatTimerHandle != NULL);
	xTimerStart(controllerRepeatTimerHandle, 0);
//	driveInfoPubRepeatTimerHandle = xTimerCreate("DriveInfoPublishRepeatTimer", DriveInfoPublishPeriod/portTICK_PERIOD_MS, pdTRUE, NULL, driveInfoPubCallBack);
//	assert_param(driveInfoPubRepeatTimerHandle != NULL);
//	xTimerStart(driveInfoPubRepeatTimerHandle, 0);
}

void difDriveTask(void * argument)
{
	for(;;)
	{

		if (ulTaskNotifyTake(pdFALSE, 2 * DifDrivePeriod/portTICK_PERIOD_MS) != 0)
		{
			//SEGGER_SYSVIEW_PrintfHost("start motor MotorFeedBackTask");
			dif_drive_controller.control_periodic();
		}
		else
		{
			//SEGGER_SYSVIEW_PrintfHost("Failed to start MotorFeedBackTask");
		}
	}
}

//void driveInfoPubTask(void * argument)
//{
//	for(;;)
//	{
//
//		if (ulTaskNotifyTake(pdFALSE, 2 * DriveInfoPublishPeriod/portTICK_PERIOD_MS) != 0)
//		{
//			//SEGGER_SYSVIEW_PrintfHost("start DriveInfoPublishTask");
//			xQueueSend(odomQueue, &dif_drive_controller.odom, 0);
//
//		}
//		else
//		{
//			//SEGGER_SYSVIEW_PrintfHost("Failed to start DriveInfoPublishTask");
//		}
//	}
//}

void difDriveCallBack(TimerHandle_t xTimer)
{
	//SEGGER_SYSVIEW_PrintfHost("DifDriveControllerRepeatTimer start");
	xTaskNotifyGive(difDriveTaskHandle);
}

//void driveInfoPubCallBack(TimerHandle_t xTimer)
//{
//	//SEGGER_SYSVIEW_PrintfHost("DriveInfoPublishRepeatTimer start");
//	xTaskNotifyGive(driveInfoPubTaskHandle);
//}

//#include "simple_math.h"
//int num = 0;
//float theta = 0;
//float sinVal;
//float cosVal;

//void test_sin(void)
//{
//	while(1)
//	{
//		num++;
//		theta = (float)num*2*simple_math::PI/128;
//		sinVal = simple_math::sin(theta);
//		cosVal = simple_math::cos(theta);
//		vTaskDelay(2/portTICK_PERIOD_MS);
//	}
//}

//float target_vel_left = 2000;
//float target_vel_right = 0;
//float cmd_lin_vel = 2000;
//float cmd_ang_vel = 3.14/6;
//float nrps;
//float vel;

//void test_duty(void * argument)
//{
//	for(;;)
//	{
//		fb_controller_left.driver->drive_motor_duty(50, FORWARD);
//		fb_controller_right.driver->drive_motor_duty(80, FORWARD);
//	}
//}
//
//void test_step_response_task(void * argument)
//{
//	int cnt = 0;
//	for(;;)
//	{
//		if (xSemaphoreTake(controllerRepeatSemPtr, 2 * DifDrivePeriod / portTICK_PERIOD_MS) == pdPASS)
//		{
//			//SEGGER_SYSVIEW_PrintfHost("start motor feedback");
//			//SEGGER_SYSVIEW_PrintfHost("motor feedback start");
//			if (cnt < 1500) {
//				dif_drive_controller.set_cmd_vel(0, 0);
//				cnt += 1;
//			} else if (cnt < 3000) {
//				dif_drive_controller.set_cmd_vel(0.6, 0);
//				cnt += 1;
//			} else {
//				cnt = 0;
//			}
//			dif_drive_controller.fb_control_periodic();
//		}
//		else
//		{
//			//SEGGER_SYSVIEW_PrintfHost("Failed to start motor feedback");
//		}
//
//	}
//}

