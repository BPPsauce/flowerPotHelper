//Soil Sensor 
#include <stdbool.h>

#define NUM_SENSORS 3
#define POT_ZERO 0
#define POT_ONE 1
#define POT_TWO 2

bool isMoist(int sensorNumber);

int getMoistureRating(int sensorNumber);