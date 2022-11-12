//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "soilSensor.h"


#define SEESAW_ADDRESS 0x49//defult seesaw I2C address 
#define SEESAW_STATUS_BASE 0x00
#define SEESAW_STATUS_TEMP 0x04
#define SEESAW_TOUCH_BASE 0x0F
#define SEESAW_TOUCH_OFFSET 0x10
#define SEESAW_EEPROM_I2C_ADDR 0x3F
#define SEESAW_EEPROM_BASE 0X0D

#define PIN 0

#define I2C_LINUX_BUS1 "/dev/i2c-1"
#define I2C_ADDR 0x37

int i2cFileDesc; 


static int initI2cBus (char* bus, int address){
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
}
    return i2cFileDesc;
}


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

static void readData(unsigned char regHigh, unsigned char regLow){
    unsigned char prefix[2];
    prefix[0] = regHigh;
    prefix[1] = regLow;

    int res = read(i2cFileDesc, prefix, 2);
    printf("Reading is: %d \n", res);

}

void readCapa(){
    readData(SEESAW_TOUCH_BASE, SEESAW_TOUCH_OFFSET+ PIN);

}

void initSoilSensor(){
    i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, I2C_ADDR);
    writeI2cReg(i2cFileDesc, 0x49, 0x11);
}