# quade
ECEN 2440 quade code

Repository for RC Quade (Quadruped robot with MSP432 as its base, controlled wirelessly by a remote control with joystick).

ECEN 2440 Final Project Team 11 - Michael L, Terry V, Wyatt S, Ethan C

Fall, 2020

All code files (original and leveraged) can be found here, along with:
  - Texas Instruments MSP432P401R LaunchPad datasheet, header file, and Technical Reference Manual (quade/Datasheet_TechRefMan/)
  - EAGLE PCB schematic and board design files for Arduino Uno shield, for remote control (quade/Arduino_Uno_Shield/)

Works Cited/Leveraged Code:
  - Tyler Davidson: IMU_TestBench code (stIMU.c/.h, pca9685.c/.h, i2c.c/.h, parts of main.c); FirstSteps code (servo.c/.h, parts of gait.c/.h)
  - Paul Stoffregen: RadioHead library (basis of our custom Arduino sketch for RF Link Transmitter)
    - https://github.com/PaulStoffregen/RadioHead
  - Texas Instruments, Inc: MSP432 Driver Library (used for ADC code)
    - adc14.c/.h, cpu.c/.h, debug.h, gpio.c/.h, interrupt.c/.h, sysctl.c/.h, sysctl_a.c/.h
    - https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP432_Driver_Library/latest/index_FDS.html
