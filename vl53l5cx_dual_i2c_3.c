#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "vl53l5cx_api.h"

#define DELAY_SLEEP  5000

VL53L5CX_Configuration dev[4];
uint8_t status, isAlive, isReady,i;
VL53L5CX_ResultsData 	Results;

char address_slave[] = {0x30, 0x31, 0x32, 0x33};


int main()
{	
	for(i=0;i<4;i++){
		status = vl53l5cx_comms_init(&dev[i].platform);
		if(status) return -1;
		usleep(DELAY_SLEEP);
	}
	
for(i=0;i<4;i++)
{
	dev[i].platform.address = address_slave[i]<<1;
}

	// dev[0].platform.address = 0x30<<1;
	// dev[1].platform.address = 0x31<<1;
	// dev[2].platform.address = 0x32<<1;
	// dev[3].platform.address = 0x33<<1;
	
	

	for(i=0;i<4;i++){
		status = vl53l5cx_set_ranging_frequency_hz(&dev[i], 50);
		if(status) return -1;
		usleep(DELAY_SLEEP);
	
	}
	

	for(i=0;i<4;i++){
		status = vl53l5cx_is_alive(&dev[i], &isAlive);
		if(!isAlive || status) return status;
		usleep(DELAY_SLEEP);

	}
	


	for(i=0;i<4;i++){
		status = vl53l5cx_init(&dev[i]);
		if(status) return status;
		usleep(DELAY_SLEEP);
	}
	

	for(i=0;i<4;i++){
		status = vl53l5cx_start_ranging(&dev[i]);
		usleep(DELAY_SLEEP);
	}
	
	for(i=0;i<4;i++){
		vl53l5cx_check_data_ready(&dev[i], &isReady);
		usleep(DELAY_SLEEP);
	}
	int loop =0;
	while(loop<20)
	{	
		for(i=0;i<4;i++)
		{
			status = vl53l5cx_check_data_ready(&dev[i], &isReady);
			if(isReady)
			{	printf("\nSensor %d:\n",i);
				
				{
				vl53l5cx_get_ranging_data(&dev[i], &Results);
				for(int a = 0; a < 16; a++)
				{
					if(a%4==0) printf("\n");

					printf("|%4d mm|",Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a]);
				}
				printf("\n");
				}
				loop++;
			}
			usleep(5000);
		}
		
	}
	
	
	for(i=0;i<4;i++)
	{
		status = vl53l5cx_stop_ranging(&dev[i]);
		vl53l5cx_comms_close(&dev[i].platform);
	}
	

	return 0;
}
