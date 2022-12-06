//soilSensor.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "soilSensor.h"
#include "utils.h"

#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_FILE_VOLTAGE5 "/sys/bus/iio/devices/iio:device0/in_voltage5_raw"

#define SOIL_SENSOR0_VOLTAGE_FILE A2D_FILE_VOLTAGE0
#define SOIL_SENSOR1_VOLTAGE_FILE A2D_FILE_VOLTAGE1
#define SOIL_SENSOR2_VOLTAGE_FILE A2D_FILE_VOLTAGE5

//Threshhold that determines water is needed
#define MOISTURE_THRESHHOLD 3500 

//Levels of moisture we can have 10 is Max Moist 0 is Dry
#define NUM_MOISTURE_LEVELS 10

//Maximum wetness Moisture Sensor can read
#define MIN_MOISTURE_READING 1000

//Maximum dryness Moisture Sensor can read
#define MAX_MOISTURE_READING 4095


static int readSoilMoisture(int sensorNumber){
    int reading = 0;
    if(sensorNumber == 0)
        reading = getAnalogReading(SOIL_SENSOR0_VOLTAGE_FILE);
    else if (sensorNumber == 1)
        reading = getAnalogReading(SOIL_SENSOR1_VOLTAGE_FILE);
    else if (sensorNumber == 2)
        reading = getAnalogReading(SOIL_SENSOR2_VOLTAGE_FILE);
    return reading;
}

bool isMoist(int sensorNumber)
{
    if(readSoilMoisture(sensorNumber) > MOISTURE_THRESHHOLD)
    { 
        return false;
    }
    return true;
}

int getMoistureRating(int sensorNumber)
{
    double moistureReading = readSoilMoisture(sensorNumber);
    // printf("sensornum: %d value: %f\n", sensorNumber, moistureReading);
    moistureReading -= MIN_MOISTURE_READING;
    double maxMoisture = MAX_MOISTURE_READING - MIN_MOISTURE_READING;
    int moistureRating = 0;
    for(int i = NUM_MOISTURE_LEVELS - 1; i > 0; i--)
    {
        if(moistureReading < i*maxMoisture/NUM_MOISTURE_LEVELS)
        {
            moistureRating++;
        }
    }
    return moistureRating;
}