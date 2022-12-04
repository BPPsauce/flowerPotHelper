#include "reminderPlayer.h"
#include "utils.h"
#include "audioMixer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "soilSensor.h"

#define REMINDER_SOUND "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"

static wavedata_t reminderSound;
static bool quit = false;

static pthread_t reminderGenerationThreadID;

static void playReminder(){
    AudioMixer_queueSound(&reminderSound);
    sleep_for_ms(500);
}

static void *reminderGenerationThread(void *_){
    while (!quit){
        for(int i = 0; i < NUM_SENSORS; i++)
        {
            if(!isMoist(i))
            {
                playReminder();
            }
        }
        sleep_for_ms(500);
    }
    return NULL;
}


void reminderPlayerInit(void){
    AudioMixer_setVolume(100);
    AudioMixer_readWaveFileIntoMemory(REMINDER_SOUND, &reminderSound);
    pthread_create(&reminderGenerationThreadID, NULL, &reminderGenerationThread, NULL);
}

void reminderPlayerStop(void){
    printf("Stopping beat player thread\n");
    quit = true;
    pthread_join(reminderGenerationThreadID, NULL);
    AudioMixer_freeWaveFileData(&reminderSound);
    printf("finished stopping beat player thread\n\n");
}