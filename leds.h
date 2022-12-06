#ifndef LEDS_H
#define LEDS_H

void setLED(char *filename, int val);
void LED_setCurrentSensor(int sensorNumber);
void LED_Init(void);
void LED_Cleanup(void);

#endif