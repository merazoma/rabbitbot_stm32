/*
 * battery_monitor.h
 *
 *  Created on: Jul 21, 2021
 *      Author: masaya
 */

#ifndef INC_BATTERY_MONITOR_H_
#define INC_BATTERY_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct {
	float capacity;
	float voltage;
	float percentage;
	uint32_t time_stamp;
} battery_state_t;

class BatteryMonitor
{

private:
	uint16_t adc_value;
	uint32_t ad_timeout;
	float adc_voltage;
	float voltage_offset;
//	float capacity_current;
//	float capacity_volt;
public:
	float battery_voltage;
	BatteryMonitor();
	void init();
	void calc_battery_state(void);
//	battery_state_t get_battery_state(void);
};

#ifdef __cplusplus
}
#endif

#endif /* INC_BATTERY_MONITOR_H_ */
