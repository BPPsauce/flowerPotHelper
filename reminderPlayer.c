#include "reminderPlayer.h"
#include "utils.h"
#include "audioMixer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "soilSensor.h"

#define REMINDER_SOUND "beatbox-wav-files/mixkit-musical-alert-notification-2309.wav"

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
            sleep_for_ms(500);
        }
    }
    return NULL;
}


void reminderPlayerInit(void){
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