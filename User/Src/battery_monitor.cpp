/*
 * battery_monitor.cpp
 *
 *  Created on: Jul 21, 2021
 *      Author: masaya
 */

#include "battery_monitor.h"
#include "adc.h"
#include "RabbitBotConfig.h"
#include "FreeRTOS.h"
#include "task.h"

const uint32_t ADCTimeOut = 300;
const float Rup = 10000;
const float Rdown = 2700;

BatteryMonitor::BatteryMonitor()
//:capacity_current(CapacityCurrent), capacity_volt(CapacityVoltage)
:battery_voltage(CapacityVoltage), voltage_offset(VoltageOffset)
{
}

void BatteryMonitor::init()
{
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		 /* Calibration Error */
		 while(1);
	}
	if (HAL_ADC_Start(&hadc1) != HAL_OK)
	{
	     /* Start Conversation Error */
		while(1);
	}
}

void BatteryMonitor::calc_battery_state(void)
{
	if (HAL_ADC_PollForConversion(&hadc1, ADCTimeOut) != HAL_OK)
	{
		 /* End Of Conversion flag not set on time */
		 while(1);
	}
	else
	{
		/* ADC conversion completed */
		adc_value = HAL_ADC_GetValue(&hadc1);
	}
	adc_voltage = (float) 3.3 * adc_value / (2 << (8 - 1));
	battery_voltage = adc_voltage * (Rup + Rdown) / Rdown + voltage_offset;
}


//battery_state_t BatteryMonitor::get_battery_state(void)
//{
//	battery_state_t battery_state;
//	battery_state.capacity = capacity_current;
//	battery_state.voltage = battery_voltage;
//	battery_state.time_stamp = (uint32_t) xTaskGetTickCount();
//	battery_state.percentage = battery_voltage / capacity_volt;
//	return battery_state;
//}
