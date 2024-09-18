/*
 * mpu6050.h
 *
 *  Created on: May 30, 2024
 *      Author: ethan
 */
#include "i2c.h"
#include <stdint.h>

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#define DEVICE_ADDRESS  			0x68 		// Slave address
#define WHO_AM_I_REG    			0x75 		// Device ID

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define REG_CONFIG_GYRO				0x1B 		// Gyro Config
#define REG_CONFIG_ACC  			0x1C 		// Accell Config
#define PWR_MGMT_1				    0x6B 		// Power Control
#define PWR_MGMT_2					0x0C

#define PLL_CLOCK_X					(1 << 0)
#define RESET						0x40 		//Resets Control Register values
#define SET_SLEEP_OFF				0b00001000  // Set sleep to 0 and disable temp
#define PWR_1_CONFIG				PLL_CLOCK_X


#define GYRO_XOUT_H     			0x43 		//Gyro measurements
#define ACCEL_XOUT_H   	 			0x3B 		// Accelerometer start address

void mpu_init (void);
void mpu_read_values (uint8_t reg);
#endif /* INC_MPU6050_H_ */
