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



    while(true){
        double x = joyStickReadX();
        double y = joyStickReadY();
        int direction = getDirection(x, y);
        int plant_number = 0;
        int rating;
        bool moist;

     
        //Center
        while(direction = 0){
            moist = isMoist(plant_number);
            if(moist){
                printSmileyFace();
            }
            else{
                printSadFace();
            }
            x = joyStickReadX();
            y = joyStickReadY();
            direction = getDirection(x, y);
        }

        //Up
        if(direction = 1){
            rating = getMoistureRating(plant_number);
            printInteger(rating);
            sleep_for_ms(1000); 
        }

        //Down
        if(direction = 2){
            printPlant(plant_number);
            sleep_for_ms(1000);
            
        }

        //Left
        if(direction = 3){
            plant_number = plant_number - 1;
            if(plant_number < 0){
                plant_number = 2;
            } 
            sleep_for_ms(100);
        }
        //Right
        if(direction = 4){
            plant_number = plant_number + 1;
            if(plant_number > 2){
                plant_number = 0;
            }
            sleep_for_ms(100); 
        }

    }

    displayCleanup();
    LED_Cleanup();
    AudioMixer_cleanup();
    reminderPlayerStop();
    
}