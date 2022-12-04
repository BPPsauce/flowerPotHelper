#include <stdio.h>
#include <stdlib.h>
#include "soilSensor.h"
#include "leds.h"
#include "reminderPlayer.h"
#include "audioMixer.h"
#include <stdbool.h>
#include "utils.h"
#include "display.h"
#include "joystick.h"


int main(int argc, char *argv[])
{
    //Initialization
    reminderPlayerInit();
    AudioMixer_init();
    LED_Init();
    displayInit();

    //Use Joystick to update numsensor for led and display threads

    displayCleanup();
    LED_Cleanup();
    AudioMixer_cleanup();
    reminderPlayerStop();
    
}