

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
#define DELAY 10000

VL53L5CX_Configuration 	Dev[4];

static int exportPin(int );
static int directionPin(int );
static int writePin(int ,int);
int pin_start();

unsigned char  address[4] = {0x30,0x31,0x32,0x33};

int main()
{

	pin_start();
    return 0;

}


int pin_start()
{
	int status;
	
	if(-1 == exportPin(PIN_BACK) || -1 == exportPin(PIN_FRONT)|| -1 == exportPin(PIN_LEFT)|| -1 == exportPin(PIN_RIGHT)) return 1;
	if(-1 == directionPin(PIN_BACK) ||-1 == directionPin(PIN_FRONT) ||-1 == directionPin(PIN_LEFT) ||-1 == directionPin(PIN_RIGHT) ) return 2;
	writePin(PIN_BACK,0);
	writePin(PIN_FRONT,0);
	writePin(PIN_LEFT,0);
	writePin(PIN_RIGHT,0);
	usleep(DELAY);


	for(int i=0;i<4;i++)
	{

	writePin(PIN_BACK,(i-0>=0?1:0));
	writePin(PIN_FRONT,(i-1>=0?1:0));
	writePin(PIN_LEFT,(i-2>=0?1:0));
	writePin(PIN_RIGHT,(i-3>=0?1:0));
	usleep(DELAY);
	printf("1");
	status = vl53l5cx_comms_init(&Dev[i].platform);
	if(status)
	{
		printf("VL53L5CX comms init failed%d\n",i);
		return -1;
	}

	status = vl53l5cx_set_i2c_address(&Dev[i], address[i]<<1);	
	vl53l5cx_comms_close(&Dev[i].platform);
	usleep(DELAY);
	}

	return 1;

}


/////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////
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
