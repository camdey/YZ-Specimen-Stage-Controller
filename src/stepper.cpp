#include "stepper.h"

void changeDirection(bool stickDirection, char axis) {
  switch (axis) {

    case 'y':
      if (stickDirection == false) {
        PORTC &= ~(1 << Y_DIR_PORTC7);
      }
      else if (stickDirection == true) {
        PORTC |= 1 << Y_DIR_PORTC7;
      }
      break;

    case 'z':
      if (stickDirection == false) {
        PORTC |= 1 << Z_DIR_PORTC6;
      }
      else if (stickDirection == true) {
        PORTC &= ~(1 << Z_DIR_PORTC6);
      }
      break;

    default:
      break;
  }
}


void moveStepper() {
  currentMicros = micros();

  if (microsSinceStep >= (microseconds / stepFrequency)) {
    if (abs(yStickPos) >= abs(zStickPos)) {
      PORTD |= 1 << Y_STEP_PORTD6;
      PORTD &= ~(1 << Y_STEP_PORTD6);
    }
    else if (abs(zStickPos) > abs(yStickPos)) {
      PORTD |= 1 << Z_STEP_PORTD7;
      PORTD &= ~(1 << Z_STEP_PORTD7);
    }
    prevStep = micros();
  }

  microsSinceStep = currentMicros - prevStep;
}


int setFrequency(int yPos, int zPos, bool frequencyModifier) {
  int frequency = 0;
  int maxFrequency = 10000; // 10,000 steps / second

  if (frequencyModifier == true) {
    maxFrequency = 1000;
  }

  // set speed
  if (yPos >= zPos) {
    frequency = map(yPos, 5, 100, 0, maxFrequency);
  }
  else if (zPos > yPos) {
    frequency = map(zPos, 5, 100, 0, maxFrequency);
  }

  return frequency;
}


void toggleOutputs(bool toggleBoth) {

  if (toggleBoth == true) {

    // disable
    if (outputsDisabled == false) {
      yStepper.disableOutputs();
      zStepper.disableOutputs();

      outputsDisabled = true;
      lastStateChange = millis();
      Serial.println("disabled both");
    }

    // enable
    else if (outputsDisabled == true) {
      yStepper.enableOutputs();
      zStepper.enableOutputs();

      outputsDisabled = false;
      lastStateChange = millis();
      Serial.println("enabled both");
    }

  }

  else if (toggleBoth == false) {

    // if all enabled, disable y
    if (yDisabled == false && zDisabled == false) {
      yStepper.disableOutputs();
      yDisabled = true;

      lastStateChange = millis();
    }

    // if y disabled, disable z and enable y
    else if (yDisabled == true && zDisabled == false) {
      yStepper.enableOutputs();
      yDisabled = false;

      zStepper.disableOutputs();
      zDisabled = true;

      lastStateChange = millis();
    }

    // if z disabled, enable z
    else if (yDisabled == false && zDisabled == true) {
      zStepper.enableOutputs();
      zDisabled = false;

      lastStateChange = millis();
    }
  }
}
