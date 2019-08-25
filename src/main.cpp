#include <SPI.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include "nunchuk.h"
#include "config.h"
#include "joystick.h"
#include "stepper.h"
#include "variableDeclarations.h"

TMC2130Stepper yDriver = TMC2130Stepper(Y_EN_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CS_PIN);
TMC2130Stepper zDriver = TMC2130Stepper(Z_EN_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CS_PIN);
AccelStepper yStepper = AccelStepper(yStepper.DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper zStepper = AccelStepper(zStepper.DRIVER, Z_STEP_PIN, Z_DIR_PIN);

volatile int yStickPos, yDirection;
volatile int zStickPos, zDirection;
bool cButton = false;
bool zButton = false;
bool outputsDisabled = false, yDisabled = false, zDisabled = false;
bool readSuccess;
long firstRead, lastRead, millisSinceRefresh;
long prevStep, currentMicros, microsSinceStep;
long currentTime, lastStateChange, millisSinceStateChange;
int stepFrequency = 100;

void setup() {
    Serial.begin(250000);
    SPI.begin();
    Wire.begin();
    Wire.setClock(280000UL);

    pinMode (Y_EN_PIN, OUTPUT);
    pinMode (Z_EN_PIN, OUTPUT);
    pinMode (Y_CS_PIN, OUTPUT);
    pinMode (Z_CS_PIN, OUTPUT);
    // set Y_DIR_PIN and Z_DIR_PIN as OUTPUT
    DDRC |= 1 << Y_DIR_DDC7;
    DDRC |= 1 << Z_DIR_DDC6;
    // set Y_STEP_PIN and Z_STEP_PIN as OUTPUT
    DDRD |= 1 << Y_STEP_DDD6;
    DDRD |= 1 << Z_STEP_DDD7;

    // set SPI slaves to ignore
    digitalWrite (Y_CS_PIN, HIGH);
    digitalWrite (Z_CS_PIN, HIGH);

    // set direction
    digitalWrite (Y_DIR_PIN, LOW);
    digitalWrite (Z_DIR_PIN, LOW);

    nunchuk_init();


  // set default driver settings
  {
    digitalWrite (Y_CS_PIN, LOW);
    silentStepConfig(yDriver);
    digitalWrite (Y_CS_PIN, HIGH);

    delay(250);

    digitalWrite (Z_CS_PIN, LOW);
    silentStepConfig(zDriver);
    digitalWrite (Z_CS_PIN, HIGH);
  }

  accelStepperConfig(yStepper, 'y');
  accelStepperConfig(zStepper, 'z');

}

void loop() {

  currentTime = millis();

  if (currentTime % 50 == 0) {

    readSuccess = checkJoystick(true);
    if (readSuccess == true) {

      if (millisSinceStateChange >= 400) {
        checkButtons();
      }

      millisSinceStateChange = currentTime - lastStateChange;

      if (outputsDisabled == false) {
        joystick();
      }
    }
  }
}
