# YZ-Specimen-Stage-Controller
### Background
This is Arduino-based controller built to work in tandem with my MacroStepper project. The MacroStepper controls an X-axis linear actuator to enable fine movements of a camera along the axis in increments ranging from 100μm down to 1.25μm.

This YZ Specimen Stage Controller is not mounted to the X-axis of the MacroStepper, rather it controls the movement of the intended specimen, or subject, for the MacroStepper. As there is no need to record position, end-stops, speed, or sync with the MacroStepper in any way, this project is considerably more simple.

## Components

 - Adafruit Feather Bluefruit 32u4
 - TMC2130 stepper driver (x2)
 - V7805-1000R  voltage regulator
 - Genuine Wii Nunchuk controller
 - 4-pin GX12 aviation plugs and sockets (x3 pairs)
 - V-Slot XY linear actuator
 - V-Slot components for mounting actuator vertically
 - Stepper motors (x2)

## Functionality

 - Choose from 2, 4, 8, 16, 32, 64, 128, or 256 microsteps
 - Control stepper speed based on Nunchuk controller input
 - Disable Y, Z, or both axis motors via controller input
 - Extra low speed mode for fine adjustment
