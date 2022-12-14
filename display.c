#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include "utils.h"
#include "soilSensor.h"

#define DISPLAY_I2C_ADDR 0x70

#define I2C_LINUX_BUS1 "/dev/i2c-1"

#define ROW0 0X00
#define ROW1 0X02
#define ROW2 0X04
#define ROW3 0X06
#define ROW4 0X08
#define ROW5 0X0A
#define ROW6 0X0C
#define ROW7 0X0E

//static pthread_t displayUpdateThreadId;
static int currentSensor = 0;
//static int stopping = 0;

unsigned char rows[] = {ROW7, ROW6, ROW5, ROW4, ROW3, ROW2, ROW1, ROW0};

// << 4 to get the one on the right
// the digit[0] and digit[1] will select the correct pattern from the table
unsigned char numberTable[10][8] = {{0x02, 0x05, 0x05, 0x05, 0x05, 0x05, 0x02, 0x00}, //0
                                    {0x02, 0x06, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00}, //1
                                    {0x07, 0x01, 0x01, 0x07, 0x04, 0x04, 0x07, 0x00}, //2
                                    {0x07, 0x01, 0x01, 0x03, 0x01, 0x01, 0x07, 0x00}, //3 
                                    {0x05, 0x05, 0x05, 0x07, 0x01, 0x01, 0x01, 0x00}, //4 
                                    {0x07, 0x04, 0x04, 0x07, 0x01, 0x05, 0x07, 0x00}, //5 
                                    {0x07, 0x04, 0x04, 0x07, 0x05, 0x05, 0x07, 0x00}, //6 
                                    {0x07, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00}, //7
                                    {0x07, 0x05, 0x05, 0x02, 0x05, 0x05, 0x07, 0x00}, //8
                                    {0x07, 0x05, 0x05, 0x07, 0x01, 0x05, 0x07, 0x00}}; //9


unsigned char smileyFace [] = {0x1E, 0X21, 0XD2, 0XC0, 0XD2, 0XCC, 0X21, 0x1E};
unsigned char sadFace [] = {0x1E, 0X21, 0XD2, 0XC0, 0XCC, 0XD2, 0X21, 0x1E};
unsigned char letterP [] = {0x20, 0x50, 0x50, 0x50, 0x60, 0x40, 0x40, 0x40};
unsigned char letterL [] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70};

//init for the i2c bus
static int initI2cBus (char* bus, int address){
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
}
    return i2cFileDesc;
}

//taken from the tutorials
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value){
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);

    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

//reset the display to empty
void resetDisplay(){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], 0x00);
    }
}

static void i2cpinsInit(){
    //init for P_18 and P_17
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
}



void printSmileyFace(){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], smileyFace[i]);
    }
}

void printSadFace(){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], sadFace[i]);
    }
}

void printPlant(int plant){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
     unsigned char value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 8; i++){
        value[i] |= numberTable[plant][i];
        //then OR together to get the final value
        value[i] |= letterP[i];
    }

    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], value[i]);
    }

}

void printLevel(int level){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    unsigned char value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 8; i++){
        value[i] |= numberTable[level][i];
        //then OR together to get the final value
        value[i] |= letterL[i];
    }

    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], value[i]);
    }
}

/*static void *displayUpdateThread(void* _)
{
    while(!stopping)
    {   
        printPlant(1);
        sleep_for_ms(1000);
        printSadFace();
        sleep_for_ms(1000);
        printSmileyFace();
        sleep_for_ms(1000);
        int moistureRating = getMoistureRating(currentSensor);
        printInteger(moistureRating);
        sleep_for_ms(1000);
    }
    return NULL;
}*/

//init the i2c pins and turn on the display
void displayInit(){
    i2cpinsInit();
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);
    //pthread_create(&displayUpdateThreadId, NULL, &displayUpdateThread, NULL);
}

void updateCurrentSensor(int numSensor)
{
    currentSensor = numSensor;
}

void displayCleanup(void)
{
    //stopping = true;
    resetDisplay();
    //pthread_join(displayUpdateThreadId, NULL);
}
