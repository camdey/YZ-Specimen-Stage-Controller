#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>
#include "variableDeclarations.h"
// #include "nunchuk/nunchuk.h"

void checkButtons();
bool checkJoystick(bool updateAll);
void joystick();
bool setDirection(int stickPos, char axis);

#endif
