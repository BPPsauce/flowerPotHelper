#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <string.h>

#define A2D_FILE_VOLTAGE2_X "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define A2D_FILE_VOLTAGE3_Y "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"

#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

//taken from assignment 2 tutorial
static int getVoltageReading(char* voltageFile ){
    // Open file
    FILE *f = fopen(voltageFile, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

double joyStickReadX(){
    int reading = getVoltageReading(A2D_FILE_VOLTAGE2_X);
    double index = ((double)reading / A2D_MAX_READING);
    return index;
}


double joyStickReadY(){
    int reading = getVoltageReading(A2D_FILE_VOLTAGE3_Y);
    double index = ((double)reading / A2D_MAX_READING);
    return index;
}

//center - 0
//up     - 1
//down   - 2
//left   - 3
//right  - 4
int getDirection(double x, double y){
    if (x <= 0.55 && x >= 0.45 && y <= 0.55 && y >= 0.45){
        //printf("center\n");
        return 0;
    }
    if (x <= 0.25){
        //printf("pressing up\n");
        return 1; //up
    }
    if (x >= 0.75){
        //printf("pressing down\n");
        return 2; //down
    }
    if (y <= 0.25){
        //printf("pressing right\n");
        return 4; //right
    }
    if (y >= 0.75){
        //printf("pressing left\n");
        return 3; //left
    }
    return 0;
}