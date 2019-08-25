/**
 * @license Nunchuk Arduino library v0.0.1 16/12/2016
 * http://www.xarg.org/2016/12/arduino-nunchuk-library/
 *
 * Copyright (c) 2016, Robert Eisele (robert@xarg.org)
 * Dual licensed under the MIT or GPL Version 2 licenses.
 **/

#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <Arduino.h>

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_DISABLE_ENCRYPTION

// Print debug information instead of a CSV stream to the serial port
// #define NUNCHUK_DEBUG

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS 0x52

void nunchuk_init_power();

void nunchuk_init();

uint8_t nunchuk_decode_byte(uint8_t x);

uint8_t nunchuk_read();

uint8_t nunchuk_zButton();

uint8_t nunchuk_cButton();

uint8_t nunchuk_joystickX_raw();

uint8_t nunchuk_joystickY_raw();

int16_t nunchuk_joystickX();

int16_t nunchuk_joystickY();

float nunchuk_joystick_angle();

uint16_t nunchuk_accelX_raw();

uint16_t nunchuk_accelY_raw();

uint16_t nunchuk_accelZ_raw();

int16_t nunchuk_accelX();

int16_t nunchuk_accelY();

int16_t nunchuk_accelZ();

float nunchuk_pitch();

float nunchuk_roll();

void nunchuk_print();

#endif
