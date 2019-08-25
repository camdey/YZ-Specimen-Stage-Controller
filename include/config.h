#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "variableDeclarations.h"

void accelStepperConfig(AccelStepper &stepper, char axis);
void silentStepConfig(TMC2130Stepper &driver);

#endif
