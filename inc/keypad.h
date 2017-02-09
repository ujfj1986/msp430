/*
* keypad.h - define the keypad module.
*
* Keypad module is the module that control the TSM12 keypad module.
* MSP430 uses i2c to connect with TSM12 module. There is another pin
* which can raise an interrupt to notify MSP430 that TSM module has
* input data.
*
* Author: Shan Jiejing
* Date: 2017-02-09
*/

#ifndef _KEYPAD_H_
#define _KEYPAD_H_
#include "pin.h"
int initKeypad(PinHandler vcc,
    PinHandler gnd,
    PinHandler irq,
    PinHandler en,
    PinHandler scl,
    PinHandler sda,
    PinHandler bg, // background led
    PinHandler led); // led

typedef void (*KeyProcess)(char key, void* context);
int registerKeyProcess(KeyProcess proc, void* context);
int unregisterKeyProcess();

#endif //_KEYPAD_H_