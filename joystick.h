#ifndef JOYSTICK_H
#define JOYSTICK_H

double joyStickReadX();
double joyStickReadY();

//incase that both x and y are triggered
//center - 0
//up     - 1
//down   - 2
//left   - 3
//right  - 4
int getDirection(double x, double y);

#endif