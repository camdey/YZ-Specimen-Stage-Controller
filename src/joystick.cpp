#include "joystick.h"
#include "stepper.h"
#include "nunchuk.h"

void checkButtons() {
  if (cButton == true && zButton == true) {
    toggleOutputs(true);
  }
  if (cButton == true && zButton == false) {
    toggleOutputs(false);
  }
}


bool checkJoystick(bool updateAll) {
  if (nunchuk_read()) {

    if (updateAll == true) {
      yStickPos = nunchuk_joystickX();
      zStickPos = nunchuk_joystickY();
      cButton = nunchuk_cButton();
      zButton = nunchuk_zButton();
    }
    else if (updateAll == false) {
      yStickPos = nunchuk_joystickX();
      zStickPos = nunchuk_joystickY();
    }

//    Serial.println(yStickPos);

    // corrects some readings being slightly over 100
    // leaves in tact reading of 128 that is indication of another issue
    if (yStickPos > 100 && yStickPos <= 105) {
      yStickPos = 100;
    }
    if (zStickPos > 100 && zStickPos <= 105) {
      zStickPos = 100;
    }

    return true;
  }
  else {
    return false;
  }
}


void joystick() {

  yDirection = setDirection(yStickPos, 'y');
  zDirection = setDirection(zStickPos, 'z');
  stepFrequency = setFrequency(abs(yStickPos), abs(zStickPos), false);

  delay(10);

  while (abs(yStickPos) >= 5 || abs(zStickPos) >= 5) {
    firstRead = millis();
    moveStepper();

    if (millisSinceRefresh >= 100) {
      readSuccess = checkJoystick(true);

      if (readSuccess == true) {
        yDirection = setDirection(yStickPos, 'y');
        zDirection = setDirection(zStickPos, 'z');

        // if holding zButton, max frequency = 1,000 steps / second
        if (zButton == true) {
          stepFrequency = setFrequency(abs(yStickPos), abs(zStickPos), true);
        }
        else {
          stepFrequency = setFrequency(abs(yStickPos), abs(zStickPos), false);
        }

      }
      else Serial.println("read failed");

      lastRead = millis();
    }
    millisSinceRefresh = firstRead - lastRead;
  }
}


bool setDirection(int stickPos, char axis) {
  bool stickDirection;

  if (stickPos <= -2) {
    stickDirection = false;
    changeDirection(stickDirection, axis);
  }
  else if (stickPos >= 2) {
    stickDirection = true;
    changeDirection(stickDirection, axis);
  }

  return stickDirection;
}
