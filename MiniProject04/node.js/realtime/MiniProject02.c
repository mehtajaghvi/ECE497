/*
 * MiniProject02.c
 *
 *  Created on: Sep 20, 2012
 *      Author: Ruffin White
 */

#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include "ITG-3200.h"


int main(int argc, char** argv){

	//variable declarations
	short int gyroID, gyroTemp, gyroX, gyroY, gyroZ;

	// I2C Variables
	char *end;
	int res, i2cbus, address, size, file;
	int daddress;

	//check that at least two arguments are passed in
	if(argc < 4){
		printf("Usage: %s <input-gpio> <output-gpio> <i2c-bus>\n", argv[0]);
		printf("polls input-gpio, and writes value to output-gpio\n");
		fflush(stdout);
		return 1;
	}

	//assign I2C values
	i2cbus   = atoi(argv[3]);
	address  = ITG3200_I2C_ADDRESS;
	file = initialize(i2cbus, address);
	zeroGyro(file);

	//set_mux_value("gpmc_a2",6);

	gyroX = readX(file);
	gyroY = readY(file);
	gyroZ = readZ(file);
	gyroTemp = readTemp(file);

	printf("gyroX: %6d\n", gyroX);
	printf("gyroY: %6d\n", gyroY);
	printf("gyroZ: %6d\n", gyroZ);
	printf("gyroTemp: %6d\n", gyroTemp);

	close(file);
	fflush(stdout);
	return 0;
}
