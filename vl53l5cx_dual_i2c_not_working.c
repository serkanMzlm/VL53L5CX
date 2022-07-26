#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>

#include "vl53l5cx_api.h"

#define DELAY 100000

VL53L5CX_Configuration dev[4];
uint8_t status, isAlive, isReady,i;
VL53L5CX_ResultsData 	Results;

int address_slave[] = {0x30, 0x31, 0x32, 0x33};


int main()
{	
	for(i=0;i<4;i++){
		status = vl53l5cx_comms_init(&dev[i].platform);
		if(status) return -1;
	}
	printf("1");

	dev[0].platform.address = 0x30<<1;
	dev[1].platform.address = 0x31<<1;
	dev[2].platform.address = 0x32<<1;
	dev[3].platform.address = 0x33<<1;
	
	printf("2");

	for(i=0;i<4;i++){
		status = vl53l5cx_set_ranging_frequency_hz(&dev[i], 50);
		if(status) return -1;
	
	}
	printf("3");

	for(i=0;i<4;i++){
		status = vl53l5cx_is_alive(&dev[i], &isAlive);
		if(!isAlive || status) return status;

	}
	printf("4");


	for(i=0;i<4;i++){
		status = vl53l5cx_init(&dev[i]);
		if(status) return status;
	}
	printf("4");

	for(i=0;i<4;i++){
		status = vl53l5cx_start_ranging(&dev[i]);
	}
		printf("5");
	for(i=0;i<4;i++){
		vl53l5cx_check_data_ready(&dev[i], &isReady);
	}
	printf("6");

	int b=0;
	while(i<20)
	{	label:

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
				i++;
			}
			usleep(5000);
		}
		
	}
	printf("\n Devam etmek icin 1 bas\n");
	scanf("%d",&b);
	if (b==1) {
		i=0;
		goto label;}
	
	for(i=0;i<4;i++)
	{
		status = vl53l5cx_stop_ranging(&dev[i]);
		vl53l5cx_comms_close(&dev[i].platform);
	}
	

	return 0;
}
