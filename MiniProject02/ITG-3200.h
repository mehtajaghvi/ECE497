/*
 * ITG3200.h
 *
 *  Created on: Sep 20, 2012
 *      Author: Ruffin White
 */

#ifndef ITG3200_H
#define ITG3200_H

/**
 * Includes
 */
#include <errno.h>
#include "i2c-dev.h"


/**
 * Defines
 */
#define ITG3200_I2C_ADDRESS 0x69 //7-bit address

//-----------
// Registers
//-----------
#define WHO_AM_I_REG    0x00
#define SMPLRT_DIV_REG  0x15
#define DLPF_FS_REG     0x16
#define INT_CFG_REG     0x17
#define INT_STATUS      0x1A
#define TEMP_OUT_H_REG  0x1B
#define TEMP_OUT_L_REG  0x1C
#define GYRO_XOUT_H_REG 0x1D
#define GYRO_XOUT_L_REG 0x1E
#define GYRO_YOUT_H_REG 0x1F
#define GYRO_YOUT_L_REG 0x20
#define GYRO_ZOUT_H_REG 0x21
#define GYRO_ZOUT_L_REG 0x22
#define PWR_MGM_REG     0x3E

//----------------------------
// Low Pass Filter Bandwidths
//----------------------------
#define LPFBW_256HZ 0x00
#define LPFBW_188HZ 0x01
#define LPFBW_98HZ  0x02
#define LPFBW_42HZ  0x03
#define LPFBW_20HZ  0x04
#define LPFBW_10HZ  0x05
#define LPFBW_5HZ   0x06

//-----------
// Offsets
//-----------
int TEMP_OUT_OFFSET;
int GYRO_XOUT_OFFSET;
int GYRO_YOUT_OFFSET;
int GYRO_ZOUT_OFFSET;




//----------------
// Read Functions
//----------------

//This function is used to initialize the gyroscope. The function returns the -errno if an error accrues.
int initialize(int i2cbus, int address){

	char filename[20];
	sprintf(filename, "/dev/i2c-%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file<0) {
		return -errno;
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) {
			return -errno;
	}

}

//This function is used to read the WHO_AM_I_REG of the gyroscope.
//Usage: int gyroID = readWhoAmI();
int readWhoAmI(int file)
{
  int data=0;
  data = i2c_smbus_read_byte_data(file, WHO_AM_I_REG);

  return data;
}


//This function is used to read the temperature of the gyroscope.
//Usage: int gyroTemp = readTemp();
int readTemp(int file)
{
  int data=0;
  data = i2c_smbus_read_byte_data(file, TEMP_OUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, TEMP_OUT_L_REG);

  return data;
}

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int xRate = readX();
int readX(int file)
{
  int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_XOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_XOUT_L_REG);

  return data;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int yRate = readY();
int readY(int file)
{
  int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_YOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_YOUT_L_REG);

  return data;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int zRate = readZ();
int readZ(int file)
{
  int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_ZOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_ZOUT_L_REG);

  return data;
}
