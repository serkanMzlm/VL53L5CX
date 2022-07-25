#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "vl53l5cx_api.h"

#define DELAY 100000

VL53L5CX_Configuration dev1,dev2;
uint8_t status, isAlive, isReady,i;
VL53L5CX_ResultsData 	Results;

char address_slave[] = {0x30, 0x31, 0x32, 0x33};



int main()
{	
	
	status = vl53l5cx_comms_init(&dev1.platform);
	if(status) return -1;

	status = vl53l5cx_comms_init(&dev2.platform);
	if(status) return -1;
	
	dev1.platform.address = 0x30<<1;
	dev2.platform.address = 0x31<<1;


	status = vl53l5cx_is_alive(&dev1, &isAlive);
	if(!isAlive || status) return status;

	status = vl53l5cx_is_alive(&dev2, &isAlive);
	if(!isAlive || status) return status;

	status = vl53l5cx_init(&dev1);
	if(status) return status;

	status = vl53l5cx_init(&dev2);
	if(status) return status;
		
	status = vl53l5cx_start_ranging(&dev1);
	status = vl53l5cx_start_ranging(&dev2);

	vl53l5cx_check_data_ready(&dev1, &isReady);
	vl53l5cx_check_data_ready(&dev2, &isReady);

	while(i<40)
	{
		if(i%2==0){
		
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

		else 
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

		usleep(50000);
	}

	status = vl53l5cx_stop_ranging(&dev1);

	vl53l5cx_comms_close(&dev1.platform);
	

	return 0;
}
































// //dev1.platform.address = 0x30<<1;





