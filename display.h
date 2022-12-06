//display.h
//functions for 8x8 LED matrix
#ifndef DISPLAY_H
#define DISPLAY_H

//print out integer
void printInteger(int reading);

void updateCurrentSensor(int numSensor);

void displayInit(void);

void printSmileyFace(void);

void printSadFace(void);

void printPlant(int plant);

void displayCleanup(void);

void resetDisplay(void);


#endif