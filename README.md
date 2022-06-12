# PalmTab
*This is currently a work-in-progress project, and everything are subject to changes. You should not expect anything to work at this moment.*

## Description
An Arduino-based mini keyboard with a joystick as mouse, for use as a media PC remote control or in other applications.
Uses a
This project is now using an Arduino Micro as controller, but will change to using STM32F103C8T6 (Blue Pill) and [CH9329](https://www.alldatasheet.com/datasheet-pdf/pdf/1148630/WCH/CH9329.html) in the future.
Only supports wired connection for now. BLE connection capabilities may be added later.

## To-Do List
- [ ] **Mouse**
  - [x] Basic readout
  - [x] Joystick calibration
  - [ ] Negative inertia filter
    - [x] Derivative control filter
    - [ ] Non-linear gain filter
  - [x] HID communication
- [ ] **Keyboard**
  - [ ] Basic readout (Key scanning)
  - [ ] HID communication
- [ ] **Menu**
  - [x] Parameter settings through serial port
  - [ ] Onboard screen menus
- [ ] **BLE capabilities**
