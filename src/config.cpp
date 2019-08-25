#include "config.h"

void accelStepperConfig(AccelStepper &stepper, char axis) {
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(6000);
  if (axis == 'y') {
    stepper.setEnablePin(Y_EN_PIN);
  }
  else if (axis == 'z') {
    stepper.setEnablePin(Z_EN_PIN);
  }
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();
}

void silentStepConfig(TMC2130Stepper &driver) {
  driver.begin();
  driver.rms_current(650);
  driver.microsteps(16);
  driver.interpolate(1);
  driver.stealthChop(1);
}
