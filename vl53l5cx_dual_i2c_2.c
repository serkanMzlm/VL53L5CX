#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "vl53l5cx_api.h"

#define DELAY 100000

VL53L5CX_Configuration dev1,dev2,dev3,dev4;
uint8_t status, isAlive, isReady,i;
VL53L5CX_ResultsData 	Results;

char address_slave[] = {0x30, 0x31, 0x32, 0x33};



int main()
{	
	//1
	status = vl53l5cx_comms_init(&dev1.platform);
	if(status) return -1;
	//2
	status = vl53l5cx_comms_init(&dev2.platform);
	if(status) return -1;
	//3
	status = vl53l5cx_comms_init(&dev3.platform);
	if(status) return -1;
	//4
	status = vl53l5cx_comms_init(&dev4.platform);
	if(status) return -1;
	
	dev1.platform.address = 0x30<<1;
	dev2.platform.address = 0x31<<1;
	dev3.platform.address = 0x32<<1;
	dev4.platform.address = 0x33<<1;

	status = vl53l5cx_set_ranging_frequency_hz(&dev1, 30);
	if(status) return -1;
	status = vl53l5cx_set_ranging_frequency_hz(&dev2, 30);
	if(status) return -1;
	status = vl53l5cx_set_ranging_frequency_hz(&dev3, 30);
	if(status) return -1;
	status = vl53l5cx_set_ranging_frequency_hz(&dev4, 30);
	if(status) return -1;


	//1
	status = vl53l5cx_is_alive(&dev1, &isAlive);
	if(!isAlive || status) return status;
	//2
	status = vl53l5cx_is_alive(&dev2, &isAlive);
	if(!isAlive || status) return status;
	//3
	status = vl53l5cx_is_alive(&dev3, &isAlive);
	if(!isAlive || status) return status;
	//4
	status = vl53l5cx_is_alive(&dev4, &isAlive);
	if(!isAlive || status) return status;

	//1
	status = vl53l5cx_init(&dev1);
	if(status) return status;
	//2
	status = vl53l5cx_init(&dev2);
	if(status) return status;
	//3
	status = vl53l5cx_init(&dev3);
	if(status) return status;
	//4
	status = vl53l5cx_init(&dev4);
	if(status) return status;
		
	status = vl53l5cx_start_ranging(&dev1);
	status = vl53l5cx_start_ranging(&dev2);
	status = vl53l5cx_start_ranging(&dev3);
	status = vl53l5cx_start_ranging(&dev4);

	vl53l5cx_check_data_ready(&dev1, &isReady);
	vl53l5cx_check_data_ready(&dev2, &isReady);
	vl53l5cx_check_data_ready(&dev3, &isReady);
	vl53l5cx_check_data_ready(&dev4, &isReady);

	while(i<80)
	{
		if(i%4==0){
		
		status = vl53l5cx_check_data_ready(&dev1, &isReady);
		if(isReady)
		{	printf("\nSensor 1:\n");
			
			{

			vl53l5cx_get_ranging_data(&dev1, &Results);
			for(int a = 0; a < 16; a++)
			{
				if(a%4==0) printf("\n");

				printf("|%4d mm|",Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a]);
			}
			printf("\n");

			

			}
			i++;

		}
		}

		else if(i%4==1) 
		{

		status = vl53l5cx_check_data_ready(&dev2, &isReady);
		
		if(isReady)
		{
			printf("\nSensor 2:\n");
			{

			vl53l5cx_get_ranging_data(&dev2, &Results);
			for(int a = 0; a < 16; a++)
			{
				if(a%4==0) printf("\n");

				printf("|%4d mm|",Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a]);
			}
			printf("\n");

			

			}
			i++;

		}



		}


		else if(i%4==2)
		{
			printf("\nSensor 3:\n");
			{

			vl53l5cx_get_ranging_data(&dev3, &Results);
			for(int a = 0; a < 16; a++)
			{
				if(a%4==0) printf("\n");

				printf("|%4d mm|",Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a]);
			}
			printf("\n");

			

			}
			i++;

		}

		else if(i%4==3)
		{
			printf("\nSensor 4:\n");
			{

			vl53l5cx_get_ranging_data(&dev4, &Results);
			for(int a = 0; a < 16; a++)
			{
				if(a%4==0) printf("\n");

				printf("|%4d mm|",Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a]);
			}
			printf("\n");

			

			}
			i++;


		}


		usleep(5000);
	}

	status = vl53l5cx_stop_ranging(&dev1);
	status = vl53l5cx_stop_ranging(&dev2);
	status = vl53l5cx_stop_ranging(&dev3);
	status = vl53l5cx_stop_ranging(&dev4);
	vl53l5cx_comms_close(&dev1.platform);
	vl53l5cx_comms_close(&dev2.platform);
	vl53l5cx_comms_close(&dev3.platform);
	vl53l5cx_comms_close(&dev4.platform);
	

	return 0;
}














