# PalmTab
~~*This is currently a work-in-progress project, and everything are subject to changes. You should not expect anything to work at this moment.*~~
**THIS REPOSITORY HAS BEEN SEMI-PERMANENTLY ABANDONED. THERE ARE NO PLANS TO BRING IT BACK IN FORESEEABLE FUTURE.**

## Description
An Arduino-based mini keyboard with a joystick as mouse, for use as a media PC remote control or in other applications.
This project is now using an Arduino Micro as controller, but will switch to use STM32F103C8T6 (Blue Pill) and [CH9329](https://www.alldatasheet.com/datasheet-pdf/pdf/1148630/WCH/CH9329.html) instead in the future.
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
