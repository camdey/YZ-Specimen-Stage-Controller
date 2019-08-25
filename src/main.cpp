#include <SPI.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include "nunchuk/nunchuk.h"

#define EN_PIN            10
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

void accelStepperConfig(AccelStepper &stepper);
void changeDirection(bool stickDirection, char stepper);
void checkButtons();
bool checkJoystick(bool updateAll);
void joystick();
void moveStepper();
bool setDirection(int stickPos, char stepper);
int setFrequency(int yPos, int zPos);
void silentStepConfig(TMC2130Stepper &driver);
void toggleOutputs(bool toggleBoth);

TMC2130Stepper yDriver = TMC2130Stepper(EN_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_CS_PIN);
TMC2130Stepper zDriver = TMC2130Stepper(EN_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_CS_PIN);
AccelStepper yStepper = AccelStepper(yStepper.DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper zStepper = AccelStepper(zStepper.DRIVER, Z_STEP_PIN, Z_DIR_PIN);

volatile int yStickPos, yDirection;
volatile int zStickPos, zDirection;
bool buttonC = false;
bool buttonZ = false;
bool outputsDisabled = false, yDisabled = false, zDisabled = false;
bool readSuccess;
long firstRead, lastRead, millisSinceRefresh;
long prevStep, currentMicros, microsSinceStep;
long currentTime, lastStateChange, millisSinceStateChange;
int stepFrequency = 100;
long microseconds = 1000000;
int numberMicrosteps[] = {2, 4, 8, 16, 32, 64, 128, 256};

void setup() {
    Serial.begin(250000);
    SPI.begin();
    Wire.begin();
    Wire.setClock(280000UL);

    pinMode (EN_PIN, OUTPUT);
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

  accelStepperConfig(yStepper);
  accelStepperConfig(zStepper);

  yStepper.enableOutputs();
  zStepper.enableOutputs();

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

void joystick() {

  yDirection = setDirection(yStickPos, 'y');
  zDirection = setDirection(zStickPos, 'z');
  stepFrequency = setFrequency(abs(yStickPos), abs(zStickPos));

  delay(10);

  while (abs(yStickPos) >= 5 || abs(zStickPos) >= 5) {
    firstRead = millis();
    moveStepper();

    if (millisSinceRefresh >= 10) {
      readSuccess = checkJoystick(false);

      if (readSuccess == true) {
        yDirection = setDirection(yStickPos, 'y');
        zDirection = setDirection(zStickPos, 'z');
        stepFrequency = setFrequency(abs(yStickPos), abs(zStickPos));
      }
      else if (readSuccess == false) {
        Serial.println("read failed");
      }

      lastRead = millis();
    }
    millisSinceRefresh = firstRead - lastRead;
  }
}

bool checkJoystick(bool updateAll) {
  if (nunchuk_read()) {

    if (updateAll == true) {
      yStickPos = nunchuk_joystickX();
      zStickPos = nunchuk_joystickY();
      buttonC = nunchuk_buttonC();
      buttonZ = nunchuk_buttonZ();
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

bool setDirection(int stickPos, char stepper) {
  bool stickDirection;

  if (stickPos <= -2) {
    stickDirection = false;
    changeDirection(stickDirection, stepper);
  }
  else if (stickPos >= 2) {
    stickDirection = true;
    changeDirection(stickDirection, stepper);
  }

  return stickDirection;
}

void changeDirection(bool stickDirection, char stepper) {
  switch (stepper) {

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

int setFrequency(int yPos, int zPos) {
  int frequency = 0;

  // set speed
  if (yPos >= zPos) {
    // max frequency = 10,000 steps / second
    frequency = map(yPos, 0, 100, 0, 10000);
  }
  else if (zPos > yPos) {
     // max frequency = 10,000 steps / second
    frequency = map(zPos, 0, 100, 0, 10000);
  }

  return frequency;
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

void silentStepConfig(TMC2130Stepper &driver) {
  driver.begin();
  driver.rms_current(650);
  driver.microsteps(32);
  driver.interpolate(1);
  driver.stealthChop(1);
}

void accelStepperConfig(AccelStepper &stepper) {
  stepper.setMaxSpeed(10000);
  stepper.setAcceleration(6000);
  stepper.setEnablePin(EN_PIN);
  stepper.setPinsInverted(false, false, true);
  stepper.enableOutputs();
}

void checkButtons() {
  if (buttonC == true && buttonZ == true) {
    toggleOutputs(true);
  }
  if (buttonC == true && buttonZ == false) {
    toggleOutputs(false);
  }
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
//      yStepper.disableOutputs();
      yDisabled == true;

      lastStateChange = millis();
      Serial.println("y disabled");
    }

    // if y disabled, disable z and enable y
    if (yDisabled == true && zDisabled == false) {
//      yStepper.enableOutputs();
      yDisabled == false;

      zStepper.disableOutputs();
      zDisabled == true;

      lastStateChange = millis();
      Serial.println("z disabled");
    }

    // if z disabled, enable z
    if (yDisabled == false && zDisabled == true) {
//      zStepper.enableOutputs();
      zDisabled == true;

      lastStateChange = millis();
      Serial.println("both enabled");
    }
  }
}
