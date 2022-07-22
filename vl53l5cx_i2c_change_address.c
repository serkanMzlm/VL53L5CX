

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vl53l5cx_api.h"

#define PIN_LEFT  17
#define PIN_RIGHT 27 
#define PIN_FRONT 22 
#define PIN_BACK  23

#define BUFFER_MAX 3
#define DIRECTION_MAX 35
#define VALUE_MAX 30
#define LOW 0

static int exportPin(int );
//static int unexportPin(int );
static int directionPin(int );
static int writePin(int ,int);


int main()
{

	if(-1 == exportPin(PIN_BACK) || -1 == exportPin(PIN_FRONT)|| -1 == exportPin(PIN_LEFT)|| -1 == exportPin(PIN_RIGHT)) return 1;
	if(-1 == directionPin(PIN_BACK) ||-1 == directionPin(PIN_FRONT) ||-1 == directionPin(PIN_LEFT) ||-1 == directionPin(PIN_RIGHT) ) return 2;
	usleep(100000);

	writePin(PIN_BACK,0);
	writePin(PIN_FRONT,0);
	writePin(PIN_LEFT,0);
	writePin(PIN_RIGHT,0);

	usleep(100000);

	VL53L5CX_Configuration 	Dev[4];
	int status;
	//////////////////////////////////////////////
    writePin(PIN_BACK,1);
	writePin(PIN_FRONT,0);
	writePin(PIN_LEFT,0);
	writePin(PIN_RIGHT,0);
	usleep(100000);

	status = vl53l5cx_comms_init(&Dev[0].platform);

	
	if(status)
	{
		printf("VL53L5CX comms init failed0\n");
		return -1;
	}
	status = vl53l5cx_set_i2c_address(&Dev[0], 0x20<<1);
    Dev[0].platform.address = 0x20;
	vl53l5cx_comms_close(&Dev[0].platform);
	usleep(100000);

	////////////////////////////////////////////////
	
	writePin(PIN_BACK,0);
	writePin(PIN_FRONT,1);
	writePin(PIN_LEFT,0);
	writePin(PIN_RIGHT,0);

	status = vl53l5cx_comms_init(&Dev[1].platform);

	
	if(status)
	{
		printf("VL53L5CX comms init failed1\n");
		return -1;
	}
	status = vl53l5cx_set_i2c_address(&Dev[1], 0x22<<1);
    Dev[1].platform.address = 0x22;

	vl53l5cx_comms_close(&Dev[1].platform);
	usleep(100000);

	////////////////////////////////////////////////
	
	writePin(PIN_BACK,0);
	writePin(PIN_FRONT,0);
	writePin(PIN_LEFT,1);
	writePin(PIN_RIGHT,0);


	status = vl53l5cx_comms_init(&Dev[2].platform);
	
	if(status)
	{
		printf("VL53L5CX comms init failed2\n");
		return -1;
	}
	status = vl53l5cx_set_i2c_address(&Dev[2], 0x24<<1);
    Dev[2].platform.address = 0x24;

	vl53l5cx_comms_close(&Dev[2].platform);
	usleep(100000);
	
	////////////////////////////////////////////////
	
	writePin(PIN_BACK,0);
	writePin(PIN_FRONT,0);
	writePin(PIN_LEFT,0);
	writePin(PIN_RIGHT,1);

	status = vl53l5cx_comms_init(&Dev[3].platform);

	
	if(status)
	{
		printf("VL53L5CX comms init failed3\n");
		return -1;
	}
	status = vl53l5cx_set_i2c_address(&Dev[3], 0x26<<1);
    Dev[3].platform.address = 0x26;
	usleep(100000);
	vl53l5cx_comms_close(&Dev[3].platform);

	writePin(PIN_BACK,1);
	writePin(PIN_FRONT,1);
	writePin(PIN_LEFT,1);
	writePin(PIN_RIGHT,1);


    return 0;

}






















////////////////////////////////////////////////////

////////////////////////////////////////////////////
/*
static int unexportPin(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd = open ("/sys/class/gpio/unexport", 01);
    if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
*/
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
static int directionPin(int pin)
{
    char path[DIRECTION_MAX];
	int fd;
    static const char s_directions_str[]  = "in\0out";

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);

    fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

    if (-1 == write(fd, &s_directions_str[3], 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

    close(fd);
	return(0);

}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
static int writePin(int pin,int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}



static int exportPin(int pin)
{

    char buffer[BUFFER_MAX];
    ssize_t bytes_written;

    int fd = open("/sys/class/gpio/export",01);

    if(fd==-1) 
    {
        printf("Failed to open unexport for writeing!\n");
        return -1;
    }

    bytes_written = snprintf(buffer,BUFFER_MAX,"%d",pin);
    write(fd,buffer,bytes_written);
    close(fd);

    return 0;

}
