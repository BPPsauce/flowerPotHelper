//WaterLogging.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "waterLogging.h"
#include "soilSensor.h"
#include "utils.h"

#define LOG_FILE "water_log.txt"

static int POTS_previousMoistureRating[3];
static pthread_t waterLoggingThreadId;
static int stopping = 0;

bool waterLogging_WriteMoistureUpdate(void)
{
    char buff[50];
    bool ret = true;
    for(int i = 0; i < NUM_SENSORS; i++)
    {
        int currentMoistureRating = getMoistureRating(i);
        sprintf(buff, "Sensor %d: Moisture Rating %d\n", i + 1, currentMoistureRating);
        ret = file_write(LOG_FILE, buff);
    }
    if(ret)
    {
        ret = file_write(LOG_FILE, "\n");
    }
    return ret;
}

bool waterLogging_WriteWateringEvent(int sensorNumber)
{
    int currentMoistureRating = getMoistureRating(sensorNumber);
    char buff[50];
    sprintf(buff, "Pot %d has been Watered! Moisture Rating is now %d\n", sensorNumber + 1, currentMoistureRating);
    return file_write(LOG_FILE, buff);
}


bool waterLogging_GotWatered(int sensorNumber)
{
    int newMoistureRating = getMoistureRating(sensorNumber);
    // printf("sensor: %d, new rating: %d, old rating: %d\n", sensorNumber, newMoistureRating, POTS_previousMoistureRating[sensorNumber]);
    if(newMoistureRating > POTS_previousMoistureRating[sensorNumber])
    {
        return true;
    }
    POTS_previousMoistureRating[sensorNumber] = newMoistureRating;
    return false;
}

static void* waterLoggingThread(void *_)
{
    int count = 0;
    while(!stopping)
    {
        for(int i = 0; i < NUM_SENSORS; i++)
        {
            // printf("Moisture Reading for Plant %d: %d\n",i, getMoistureRating(i));
            if(waterLogging_GotWatered(i))
            {
                printf("%d Watered!\n", i);
                waterLogging_WriteWateringEvent(i);
            }
        }
        // printf("%d\n", getMoistureRating(2));
        sleep_for_ms(500);

        count++;
        if(count == 2)
        {
            printf("Calling moisture update\n");
            waterLogging_WriteMoistureUpdate();
            count = 0;
        }
    }
    return NULL;
}

void waterLogging_Init(void)
{
    //get initial sensor values for later comparison
    for(int i = 0; i < NUM_SENSORS; i++)
    {
        POTS_previousMoistureRating[i] = getMoistureRating(i);
    }
    file_write(LOG_FILE, "\n================= PROGRAM START=================\n");
    pthread_create(&waterLoggingThreadId, NULL, &waterLoggingThread, NULL);
}

void waterLogging_Cleanup(void)
{
    stopping = 1;
    pthread_join(waterLoggingThreadId, NULL);
}