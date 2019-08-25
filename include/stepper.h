#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "variableDeclarations.h"

void changeDirection(bool stickDirection, char axis);
void moveStepper();
int setFrequency(int yPos, int zPos, bool frequencyModifier);
void toggleOutputs(bool toggleBoth);

#endif
