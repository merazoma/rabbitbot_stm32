/*
 * spi_imu.c
 *
 *  Created on: May 4, 2021
 *      Author: masaya
 */
#include <spi_imu.h>
#include "spi.h"

#define WHO_AM_I_VAL 0x12

uint8_t who_am_i;

void init_imu(){

	MX_SPI1_Init();
	writeByte(CONFIG, 0x00);
	writeByte(PWR_MGMT_1, 0x80);
	writeByte(PWR_MGMT_1, 0x01);
	who_am_i = readByte(WHO_AM_I);
//	if (who_am_i != WHO_AM_I_VAL) {
//		while(1);
//	}

}

uint8_t readByte(uint8_t reg)
{
	uint8_t rx_data[2];
	uint8_t tx_data[2];

	tx_data[0] = reg | 0x80;
	tx_data[1] = 0x00;  // dummy

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
	__HAL_SPI_ENABLE(&hspi1);
	HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);

	return rx_data[1];
}

void writeByte(uint8_t reg, uint8_t data)
{
	uint8_t rx_data[2];
	uint8_t tx_data[2];

	tx_data[0] = reg & 0x7F;
	tx_data[1] = data;  // write data
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
	__HAL_SPI_ENABLE(&hspi1);
	HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 2, 1);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 1);
}
