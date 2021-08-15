/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "freertos.h"
#include "task.h"
#include "RabbitBotConfig.h"
#include "main.h"
#include "timers.h"
#include "semphr.h"
#include "tim.h"
#include "spi_imu.h"
#include "control_dif_drive.h"
#include "rosserial.h"


/* Periodic Task -----------------------------------------------*/
/* Task */
void SPITask(void * argument);
void DiagTask(void * argument);
TaskHandle_t IMUTaskHandle;
TaskHandle_t diagTaskHandle;

/* Timer */
TimerHandle_t repeatIMUHandle;
TimerHandle_t repeatDiagHandle;
void repeatIMUCallBack(TimerHandle_t xTimer);
void repeatDiagCallBack(TimerHandle_t xTimer);

/* Init func -----------------------------------------------*/
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
//void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
//{
//   /* Run time stack overflow checking is performed if
//   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
//   called if a stack overflow is detected. */
//	while(1);
//}
#ifdef __cplusplus
 extern "C" {
#endif
void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
	while(1);
}

uint32_t HAL_GetTick(void)
{
  return (uint32_t) xTaskGetTickCount();
}

#ifdef __cplusplus
 }
#endif
/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	init_rosserial();
	init_dif_drive_task();
	if (xTaskCreate(DiagTask, "DiagTask", 96, NULL, tskIDLE_PRIORITY+1, &diagTaskHandle) != pdPASS)
	{
		while(1);
	}
//	if (xTaskCreate(SPITask, "SPI IMU Task", 96, NULL, tskIDLE_PRIORITY+2, &IMUTaskHandle) != pdPASS)
//	{
//		while(1);
//	}

//	repeatIMUHandle = xTimerCreate("IMURepeatTimer", IMUPeriod/portTICK_PERIOD_MS, pdTRUE, NULL, repeatIMUCallBack);
//	assert_param(repeatIMUHandle != NULL);
//	xTimerStart(repeatIMUHandle, 0);
	repeatDiagHandle = xTimerCreate("DiagRepeatTimer", DiagPeriod/portTICK_PERIOD_MS, pdTRUE, NULL, repeatDiagCallBack);
	assert_param(repeatDiagHandle != NULL);
	xTimerStart(repeatDiagHandle, 0);

}

//#include "adc.h"
//uint16_t uhADCxConvertedValue = 0;

//uint32_t HAL_GetTick(void)
//{
//  return (uint32_t) xTaskGetTickCount();
//}

#include "battery_monitor.h"
BatteryMonitor battery_monitor = BatteryMonitor();

void DiagTask(void * argument)
{
	battery_monitor.init();
	for(;;)
	{
		if (ulTaskNotifyTake(pdFALSE, 2 * DiagPeriod / portTICK_PERIOD_MS) != 0)
		{
			battery_monitor.calc_battery_state();
		}
		else
		{
			while(1);
			//SEGGER_SYSVIEW_PrintfHost("Failed to start diag task");
		}

	}
}

//void DiagTask(void * argument)
//{
//	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
//	{
//		 /* Calibration Error */
//		 while(1);
//	}
//	if (HAL_ADC_Start(&hadc1) != HAL_OK)
//	{
//	     /* Start Conversation Error */
//		while(1);
//	}
//
//	for(;;)
//	{
//		if (ulTaskNotifyTake(pdFALSE, 2 * DiagPeriod / portTICK_PERIOD_MS) != 0)
//		{
//			 if (HAL_ADC_PollForConversion(&hadc1, 1000) != HAL_OK)
//			 {
//			   /* End Of Conversion flag not set on time */
//			   while(1);
//			 }
//			 else
//			 {
//			   /* ADC conversion completed */
//			   /*##-5- Get the converted value of regular channel  ######################*/
//			   uhADCxConvertedValue = HAL_ADC_GetValue(&hadc1);
//			 }
//		}
//		else
//		{
//			//SEGGER_SYSVIEW_PrintfHost("Failed to start diag task");
//		}
//
//	}
//}

uint8_t who_am_i;

void SPITask(void * argument)
{
	for(;;)
	{
		if (ulTaskNotifyTake(pdFALSE, 2 * IMUPeriod / portTICK_PERIOD_MS) != 0)
		{
			//SEGGER_SYSVIEW_PrintfHost("spi imu task start");
			who_am_i = readByte(WHO_AM_I);
		}
		else
		{
			//SEGGER_SYSVIEW_PrintfHost("Failed to start spi imu task");
		}

	}

}

void repeatIMUCallBack(TimerHandle_t xTimer)
{
	xTaskNotifyGive(IMUTaskHandle);
}

void repeatDiagCallBack(TimerHandle_t xTimer)
{
	xTaskNotifyGive(diagTaskHandle);
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
