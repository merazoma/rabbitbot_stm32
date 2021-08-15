/*
 * spi_imu.h
 *
 *  Created on: May 4, 2021
 *      Author: masaya
 */

#ifndef INC_SPI_IMU_H_
#define INC_SPI_IMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B

void init_imu();
uint8_t readByte(uint8_t reg);
void writeByte(uint8_t reg, uint8_t data);


#ifdef __cplusplus
}
#endif

#endif /* INC_SPI_IMU_H_ */
