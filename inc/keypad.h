/*
* keypad.h - define the keypad module.
* Keypad module will initialize the TSM12 keypad mode,
* and when there is a key event, it will call the callback
* function.
*
* Author: Shan Jiejing
* Date : 2017-02-09
*/
#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "pin.h"

int initKeypad(PinHandler vcc,
    PinHandler gnd,
    PinHandler en,
    PinHandler scl,
    PinHandler sda,
    PinHandler bg,
    PinHandler led);

typedef void (*KeyProcess)(char key, void* context);
int registerKeyProcess(KeyProcess proc, void* context);
int unregisterKeyProcess();

#endif //_KEYPAD_H_