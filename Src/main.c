#include <stdio.h>
#include <stdint.h>
/*#include <stdlib.h>*/

#include "stm32f4xx.h"
#include "uart.h"
#include "mpu6050.h"
#include "systick.h"

#define CALIBRATION_READINGS	500

#define GPIOAEN				   (1U<<0)
#define GPIOA_5				   (1U<<5)
#define LED_PIN				   GPIOA_5

int16_t x,y,z,x2,y2,z2; 								// Initial gyro and accel
int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0; 	// Gyro Offsets
int16_t acx_offset = 0, acy_offset = 0, acz_offset = 0; // Accel Offsets

/* Holds the final gyro angles */
double ax;
double ay;
double az;

/* Holds the final accel values */
double acx;
double acy;
double acz;

/* Velocity and Distance */
double vel_x = 0, vel_y = 0, vel_z = 0;
double dist_x = 0, dist_y = 0, dist_z = 0;
double dt = 0.01; // Adjust baed on sampling rates

extern uint8_t data_rec[6];

void calibrateMPU6050(void);

int main(void)
{

	/*	1.Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*	2.Set PA5 as output pin*/
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	/* Initialize I2C, MPU, Calibrate, and UART */
	mpu_init();
	calibrateMPU6050();
	uart2_tx_init();

	while(1)
		{

			/* Read in Gyro Values */
			 mpu_read_values(GYRO_XOUT_H);

			 x = (int16_t)(data_rec[0] << 8 | data_rec[1]);
			 y = (int16_t)(data_rec[2] << 8 | data_rec[3]);
			 z = (int16_t)(data_rec[4] << 8 | data_rec[5]);

			 x = x  - gx_offset;
			 y = y  - gy_offset;
			 z = z -  gz_offset;
			 /* Gyro Degrees */
			 ax = x / 131.0;
			 ay = y / 131.0;
			 az = z / 131.0;
			 /* gyro_x = (int)ax; */

			 /* Read in Accel Values */
			 mpu_read_values(ACCEL_XOUT_H);

			 x2 = (int16_t)(data_rec[0] << 8 | data_rec[1]);
			 y2 = (int16_t)(data_rec[2] << 8 | data_rec[3]);
			 z2 = (int16_t)(data_rec[4] << 8 | data_rec[5]);

			 x2 = x2 - acx_offset;
			 y2 = y2 - acy_offset;
			 z2 = z2 - acz_offset;

			 /* Number is from datasheet */
			 acx =  x2 ;
			 acy =  y2 ;
			 acz =  z2 ;

			 vel_x += acx * dt;
			 vel_y += acy * dt;
			 vel_z += acz * dt;

			 dist_x += vel_x * dt;
			 dist_y += vel_y * dt;
			 dist_z += vel_z * dt;

			 printf("10ms passed !! \n\r");
			 GPIOA->ODR ^= LED_PIN;
			 systickDelayMs(10);

		}

}

void calibrateMPU6050(void){

	int16_t sum_gx = 0;
	int16_t sum_gy = 0;
	int16_t sum_gz = 0;
	int16_t sum_acx = 0;
	int16_t sum_acy = 0;
	int16_t sum_acz = 0;

	/* Delay so the values can balance out */
	systickDelayMs(2500);

	for(int i = 0; i < CALIBRATION_READINGS; i++){

		 mpu_read_values(GYRO_XOUT_H);

		 sum_gx += (int16_t)(data_rec[0] << 8 | data_rec[1]);
		 sum_gy += (int16_t)(data_rec[2] << 8 | data_rec[3]);
		 sum_gz += (int16_t)(data_rec[4] << 8 | data_rec[5]);

		 mpu_read_values(ACCEL_XOUT_H);

		 sum_acx += (int16_t)(data_rec[0] << 8 | data_rec[1]);
		 sum_acy += (int16_t)(data_rec[2] << 8 | data_rec[3]);
		 sum_acz += (int16_t)(data_rec[4] << 8 | data_rec[5]);

		 systickDelayMs(5);
	}

	 gx_offset = sum_gx / CALIBRATION_READINGS;
	 gy_offset = sum_gy / CALIBRATION_READINGS;
	 gz_offset = sum_gz / CALIBRATION_READINGS;

	 acx_offset = sum_acx / CALIBRATION_READINGS;
	 acy_offset = sum_acy / CALIBRATION_READINGS;
	 acz_offset = sum_acz / CALIBRATION_READINGS - 16384; // assuming Z-axis is 1g;
}




