#ifndef VARIABLE_DECLARATIONS_H
#define VARIABLE_DECLARATIONS_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>

#define Y_EN_PIN          10
#define Z_EN_PIN          A2
#define Y_DIR_PIN         13
#define Y_STEP_PIN        12
#define Y_CS_PIN          11
#define Z_DIR_PIN         5
#define Z_STEP_PIN        6
#define Z_CS_PIN          9
#define MOSI_PIN          16
#define MISO_PIN          14
#define SCK_PIN           15
//--------
#define Y_DIR_DDC7        7
#define Y_DIR_PORTC7      7
#define Y_STEP_DDD6       6
#define Y_STEP_PORTD6     6
//--------
#define Z_DIR_DDC6        6
#define Z_DIR_PORTC6      6
#define Z_STEP_DDD7       7
#define Z_STEP_PORTD7     7
#define microseconds      1000000

extern TMC2130Stepper     yDriver;
extern TMC2130Stepper     zDriver;
extern AccelStepper       yStepper;
extern AccelStepper       zStepper;

extern volatile int       yStickPos, yDirection;
extern volatile int       zStickPos, zDirection;
extern bool               cButton;
extern bool               zButton;
extern bool               outputsDisabled, yDisabled, zDisabled;
extern bool               readSuccess;
extern long               firstRead, lastRead, millisSinceRefresh;
extern long               prevStep, currentMicros, microsSinceStep;
extern long               currentTime, lastStateChange, millisSinceStateChange;
extern int                stepFrequency;

#endif
