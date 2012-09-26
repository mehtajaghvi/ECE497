/*
 * ITG3200.h
 *
 *  Created on: Sep 20, 2012
 *      Author: Ruffin White
 */

/**
 * Includes
 */
#include <errno.h>
#include "i2c-dev.h"


/**
 * Defines
 */
#define ITG3200_I2C_ADDRESS 0x69 //7-bit address that Gyro is originally configured by the break-out board

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
short int TEMP_OUT_OFFSET = 0;
short int GYRO_XOUT_OFFSET = 0;
short int GYRO_YOUT_OFFSET = 0;
short int GYRO_ZOUT_OFFSET = 0;




//----------------
// Read Functions
//----------------

//This function is used to initialize the gyroscope. The function returns the -errno if an error accrues.
short int initialize(int i2cbus, int address){

	char filename[20];
	int file;
	sprintf(filename, "/dev/i2c-%d", i2cbus);
	file = open(filename, O_RDWR);
	if (file<0) {
		return -errno;
	}

	if (ioctl(file, I2C_SLAVE, address) < 0) {
			return -errno;
	}
	return file;

}

//This function is used to initialize the gyroscope. The function returns the -errno if an error accrues.
short int zeroGyro(int file){

	GYRO_XOUT_OFFSET = readX(file);
	GYRO_YOUT_OFFSET = readY(file);
	GYRO_ZOUT_OFFSET = readZ(file);

}

//This function is used to read the WHO_AM_I_REG of the gyroscope.
//Usage: int gyroID = readWhoAmI();
short int readWhoAmI(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, WHO_AM_I_REG);

  return data;
}

//This function is used to write the WHO_AM_I_REG of the gyroscope.
//Usage: data = readWhoAmI(data);
short int writeWhoAmI(int file, short int data)
{
  i2c_smbus_write_byte_data(file,WHO_AM_I_REG,data);

  return data;
}


//This function is used to read the SMPLRT_DIV_REG of the gyroscope.
short int readSmplrtDiv(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, SMPLRT_DIV_REG);

  return data;
}

//This function is used to write the SMPLRT_DIV_REG of the gyroscope.
short int writeSmplrtDiv(int file, short int data)
{
  i2c_smbus_write_byte_data(file,SMPLRT_DIV_REG,data);

  return data;
}

//This function is used to read the DLPF_FS_REG of the gyroscope.
short int readDlpfFs(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, DLPF_FS_REG);

  return data;
}

//This function is used to write the DLPF_FS_REG of the gyroscope.
short int writeDlpfFs(int file, short int data)
{
  i2c_smbus_write_byte_data(file,DLPF_FS_REG,data);

  return data;
}

//This function is used to read the INT_CFG_REG of the gyroscope.
short int readIntCfg(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, INT_CFG_REG);

  return data;
}

//This function is used to write the INT_CFG_REG of the gyroscope.
short int writeIntCfg(int file, short int data)
{
  i2c_smbus_write_byte_data(file,INT_CFG_REG,data);

  return data;
}

//This function is used to read the INT_STATUS of the gyroscope.
short int readIntStatus(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, INT_STATUS);

  return data;
}


//This function is used to read the temperature of the gyroscope.
//Usage: int gyroTemp = readTemp();
short int readTemp(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, TEMP_OUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, TEMP_OUT_L_REG);

  return data;
}

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int xRate = readX();
short int readX(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_XOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_XOUT_L_REG);

  return data - GYRO_XOUT_OFFSET;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int yRate = readY();
short int readY(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_YOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_YOUT_L_REG);

  return data - GYRO_YOUT_OFFSET;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int zRate = readZ();
short int readZ(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, GYRO_ZOUT_H_REG)<<8;
  data |= i2c_smbus_read_byte_data(file, GYRO_ZOUT_L_REG);

  return data - GYRO_ZOUT_OFFSET;
}

//This function is used to read the PWR_MGM_REG of the gyroscope.
short int readPwrMgm(int file)
{
  short int data=0;
  data = i2c_smbus_read_byte_data(file, PWR_MGM_REG);

  return data;
}

//This function is used to write the PWR_MGM_REG of the gyroscope.
short int writePwrMgm(int file, short int data)
{
  i2c_smbus_write_byte_data(file,PWR_MGM_REG,data);

  return data;
}
