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

int loop=1;


void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	loop = 0;
}

int main(int argc, char** argv){

	//variable declarations
	struct pollfd fdset[1];
	int nfds = 1;
	int timeout = 1000;
	int rc;
	char* buf[MAX_BUF];
	int gpio1, gpio2;
	int gpio1_fd, gpio2_fd;
	int gpio2_value = 0;
	int pattern =0;
	int value =0;

	int freq = 10;
	int duty = 25;

	int gyroID, gyroTemp, gyroX, gyroY, gyroZ;

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


	//set signal handler for Ctrl + C
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	//assign gpio values
	gpio1 = atoi(argv[1]);
	gpio2 = atoi(argv[2]);


	//assign I2C values
	i2cbus   = atoi(argv[3]);
	address  = ITG3200_I2C_ADDRESS;
	initialize(i2cbus, address);



	//argument 1 will be input
	export_gpio(gpio1);
	set_gpio_direction(gpio1, "in");
	set_gpio_edge(gpio1, "falling");
	gpio1_fd = gpio_fd_open(gpio1);

	//argument 2 will be output
	export_gpio(gpio2);
	set_gpio_direction(gpio2, "out");
	set_gpio_value(gpio2, gpio2_value);
	gpio2_fd = gpio_fd_open(gpio2);

	set_mux_value("gpmc_a2",6);


	while(loop){
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = gpio1_fd;
		fdset[0].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);

		if (rc < 0){
			printf("\npoll() failed!\n");
		}

		if (rc == 0){
			printf(".");
		}

		if((fdset[0].revents & POLLPRI) == POLLPRI) {
			read(fdset[0].fd, buf, MAX_BUF);
			printf("interrupt value=%c\n", buf[0]);
			pattern++;
		if(pattern == 4){
		pattern = 0;
		}
		}

		switch(pattern){

			// blink led
			case 0:
				printf("Case 0\n");
				value = read_ain("ain6");
				printf("Voltage: %d\n",value);
				set_pwm("ehrpwm.1:0",10,25);
				if(gpio2_value){
					gpio2_value = 0;
				}
				else{
					gpio2_value = 1;
				}
				set_gpio_value(gpio2, gpio2_value);
				break;

			//PWM output
			case 1:

				gyroID = readWhoAmI(file);

				printf("gyroID: 0x%02x (%d)\n", gyroID, gyroID);
				break;

			//Read Gyro Temperature
			case 2:
				printf("Case 2\n");

				gyroTemp = readTemp(file);

				printf("gyroTemp: 0x%02x (%d)\n", gyroTemp, gyroTemp);
				break;

			//Read Gyro XYZ
			case 3:
				printf("Case 3\n");

				gyroX = readX(file);
				gyroY = readY(file);
				gyroZ = readZ(file);

				printf("gyroX: 0x%02x (%d)\n", gyroX, gyroX);
				printf("gyroY: 0x%02x (%d)\n", gyroY, gyroY);
				printf("gyroZ: 0x%02x (%d)\n", gyroZ, gyroZ);
				break;

			default:
				break;
			}
		}
	close(file);
	gpio_fd_close(gpio1_fd);
	gpio_fd_close(gpio2_fd);
	unexport_gpio(gpio1);
	unexport_gpio(gpio2);
	fflush(stdout);
	return 0;
}
