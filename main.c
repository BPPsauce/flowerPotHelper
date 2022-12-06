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
#define GREEN_BUTTON "/sys/class/gpio/gpio65/value"



int main(int argc, char *argv[])
{
    //Initialization
    reminderPlayerInit();
    AudioMixer_init();
    LED_Init();
    displayInit();

    configGreenButt();
    int greenbutton = 0;
    greenbutton = readFromFile(GREEN_BUTTON);



    while(greenbutton == 0){
        double x = joyStickReadX();
        double y = joyStickReadY();
        int direction = getDirection(x, y);
        int plant_number = 0;
        int rating;
        bool moist;

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
            printPlant(plant_number);
            break;
        case 4: //right
            plant_number = plant_number + 1;
            if(plant_number > 2){
                plant_number = 0;
            }
            printPlant(plant_number);
            break;
        default:
            printf("Joystick reading not correct!\n");
            break;
        }
        greenbutton = readFromFile(GREEN_BUTTON);
    }

    printf("Shutting down system\n");
    displayCleanup();
    LED_Cleanup();
    AudioMixer_cleanup();
    reminderPlayerStop();
    
}