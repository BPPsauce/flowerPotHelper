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
#include "waterLogging.h"
#define GREEN_BUTTON "/sys/class/gpio/gpio65/value"

//black 1
//green 0


int main(int argc, char *argv[])
{
    //Initialization
    reminderPlayerInit();
    AudioMixer_init();
    LED_Init();
    displayInit();
    waterLogging_Init();
    

    configGreenButt();
    int greenbutton = 0;
    greenbutton = readFromFile(GREEN_BUTTON);
    int direction = 0;
    int plant_number = 0;
    int rating = 0;
    bool moist = false;

    while(greenbutton == 0){
        direction = getDirection(joyStickReadX(), joyStickReadY());

        switch (direction)
        {
        case 0: //center
            moist = isMoist(plant_number);
            if(moist){
                printSmileyFace();
            }
            else{
                printSadFace();
            }
            break;
        case 1: //up
            rating = getMoistureRating(plant_number);
            printInteger(rating);
            break;
        case 2: //down 
            printPlant(plant_number);
            break;
        case 3: //left
            plant_number = plant_number - 1;
            if(plant_number < 0){
                plant_number = 2;
            }
            LED_setCurrentSensor(plant_number);
            printPlant(plant_number);
            break;
        case 4: //right
            plant_number = plant_number + 1;
            if(plant_number > 2){
                plant_number = 0;
            }
            LED_setCurrentSensor(plant_number);
            printPlant(plant_number);
            break;
        default:
            printf("Joystick reading not correct!\n");
            break;
        }

        greenbutton = readFromFile(GREEN_BUTTON);
        sleep_for_ms(300);
    }

    printf("Shutting down system\n");
    waterLogging_Cleanup();
    LED_Cleanup();
    AudioMixer_cleanup();
    reminderPlayerStop();
    //displayCleanup();
    
}