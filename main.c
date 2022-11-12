#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "soilSensor.h"



int main(int argc, char *argv[])
{
    initSoilSensor();
    readCapa();
}