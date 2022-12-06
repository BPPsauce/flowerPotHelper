//display.h
//functions for 8x8 LED matrix
#ifndef DISPLAY_H
#define DISPLAY_H

void updateCurrentSensor(int numSensor);

void displayInit(void);

void printSmileyFace(void);

void printSadFace(void);

void printPlant(int plant);

void printLevel(int level);

void displayCleanup(void);

void resetDisplay(void);


#endif