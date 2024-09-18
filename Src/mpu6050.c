#include "mpu6050.h"

char data;
uint8_t data_rec[6];

void mpu_read_address (uint8_t reg)
{
	 I2C1_byteRead( DEVICE_ADDRESS, reg, &data);

}

void mpu_write (uint8_t reg, char value)
{
	char data[1];
	data[0] = value;

	I2C1_burstWrite( DEVICE_ADDRESS, reg,1, data) ;
}

void mpu_read_values (uint8_t reg)
{
	I2C1_burstRead(DEVICE_ADDRESS, reg, 6,(char *)data_rec);

}

/* Still many more improvements I can make to the initialization of the MPU6050's performance */
void mpu_init (void)
{
	/*Enable I2C*/
	I2C1_init();

	mpu_read_address(WHO_AM_I_REG); // reads 104 which is correct

	/*Gyro 250 deg*/
	mpu_write(REG_CONFIG_GYRO, MPU6050_GYRO_FS_250);

	/* Accel Write 2g */
	mpu_write(REG_CONFIG_ACC, MPU6050_ACCEL_FS_2);

	/*Resets the entire device */
	mpu_write (PWR_MGMT_1, RESET);

	/* Enable PLL_X gyro clock */
	mpu_write(PWR_MGMT_1, PWR_1_CONFIG);

}

