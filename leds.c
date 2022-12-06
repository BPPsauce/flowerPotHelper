//LED .c 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "leds.h"
#include "utils.h"
#include "soilSensor.h"

#define GPIO_EXPORT_FILE "/sys/class/gpio/export"

#define RED_LED_CFG_CMD "config-pin p8.11 gpio"
#define RED_LED_VAL "/sys/class/gpio/gpio45/value"
#define RED_LED_DIR "/sys/class/gpio/gpio45/direction"
#define RED_LED_GPIO_NUM 45

#define YELLOW_LED_CFG_CMD "config-pin p8.12 gpio"
#define YELLOW_LED_VAL "/sys/class/gpio/gpio44/value"
#define YELLOW_LED_DIR "/sys/class/gpio/gpio44/direction"
#define YELLOW_LED_GPIO_NUM 44

#define GREEN_LED_CFG_CMD "config-pin p8.13 gpio"
#define GREEN_LED_VAL "/sys/class/gpio/gpio23/value"
#define GREEN_LED_DIR "/sys/class/gpio/gpio23/direction"
#define GREEN_LED_GPIO_NUM 23

#define WHITE_LED_CFG_CMD "config-pin p8.14 gpio"
#define WHILE_LED_VAL "/sys/class/gpio/gpio26/value"
#define WHITE_LED_DIR "/sys/class/gpio/gpio26/direction"
#define WHITE_LED_GPIO_NUM 26

#define LED_ON 1
#define LED_OFF 0

static pthread_t ledThreadID;
static bool stopping = 0;
static int currentSensor = 0;

static void configLED(char* dir, char* configCmd, int gpioNum)
{        
    runCommand(configCmd);

	FILE *pFile = fopen(GPIO_EXPORT_FILE, "w");
	if (pFile == NULL) {
		printf("ERROR: Unable to open export file.\n");
		exit(1);	
	}	
	fprintf(pFile, "%d", gpioNum);
	fclose(pFile);
	sleep_for_ms(50);

	FILE *pFile1 = fopen(dir, "w");
	if (pFile1 == NULL) {
		printf("ERROR: Unable to open direction file.\n");
		exit(1);
	}
	fprintf(pFile1, "%s", "out");
	fclose(pFile1);
}

void setLED(char *filename, int val)
{
	FILE *pFile = fopen(filename, "w");
	if (pFile == NULL)
	{
		printf("ERROR to open file (%s) for write\n", filename);
		exit(-1);
	}
    fprintf(pFile, "%d", val);
    fclose(pFile);
}

static void *ledUpdateThread(void *_)
{
    while(!stopping)
    {
        if(!isMoist(currentSensor))
        {
            setLED(GREEN_LED_VAL, LED_OFF);
            setLED(RED_LED_VAL, LED_ON);
        }
        else
        {
            setLED(RED_LED_VAL, LED_OFF);
            setLED(GREEN_LED_VAL, LED_ON);
        }
        sleep_for_ms(200);
    }
    return NULL;
}

void setCurrentSensor(int sensorNum)
{
    currentSensor = sensorNum;
}

void LED_Init(void)
{
    configLED(RED_LED_DIR, RED_LED_CFG_CMD, RED_LED_GPIO_NUM);
    configLED(YELLOW_LED_DIR, YELLOW_LED_CFG_CMD, YELLOW_LED_GPIO_NUM);
    configLED(GREEN_LED_DIR, GREEN_LED_CFG_CMD, GREEN_LED_GPIO_NUM);
    configLED(WHITE_LED_DIR, WHITE_LED_CFG_CMD, WHITE_LED_GPIO_NUM);
    pthread_create(&ledThreadID, NULL, &ledUpdateThread, NULL);
}

void LED_Cleanup(void)
{
    stopping = true;
    pthread_join(ledThreadID, NULL);
}
