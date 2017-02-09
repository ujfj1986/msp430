/*
* keypad.c - implement the keypad module.
*
* Author: Shan Jiejing
* Date: 2017-02-09
*/
#include  <msp430x14x.h>
#include "keypad.h"
#include <string.h>
#include "i2c.h"

typedef struct Keypad {
    PinHandler vcc;
    PinHandler gnd;
    PinHandler irq;
    PinHandler en;
    PinHandler scl;
    PinHandler sda;
    PinHandler bg;
    PinHandler led;
    KeyProcess proc;
    void* context;
} Keypad;

static Keypad gKey;

#define KEYPAD_I2C_ADDR 0xD0
#define KEYPAD_I2C_REG_OUTPUT1 0x10
#define KEYPAD_I2C_REG_OUTPUT2 0x11
#define KEYPAD_I2C_REG_OUTPUT3 0x12

static void powerOnKeypad() {
    setPinValue(gKey.vcc, 1);
    setPinValue(gKey.gnd, 0);
    setPinValue(gKey.en, 1);
}

static void powerOffKeypad() {
    setPinValue(gKey.vcc, 0);
}

static int keypadIrqHandler(PinHandler irq, void* context) {
    //dual with keypad irq event
    char output[3] = "\0";
    int ret = 0;
    int i = 0;
    int j = 0;

    setPinValue(gKey.en, 0);
    //TODO: verify the registers of output.
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT1, &output[0]);
    if (ret) goto error;
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT2, &output[1]);
    if (ret) goto error;
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT3, &output[2]);
    if (ret) goto error;
    
    for (i = 0; i < 3; i++) {
        if (0 == output[i]) continue;
        for (j = 0; j < 4; j++) {
            if (0 != (output[i] & (0x11 << (2*j)))) break;
        }
        break;
    }
    if (3 == i) goto error;
    //mapping the output to the key.
    //TODO: call the call back function to dual with the key event.
    return 0;
error:
    setPinValue(gKey.en, 1);
    return ret;
}

int initKeypad(PinHandler vcc,
    PinHandler gnd,
    PinHandler irq,
    PinHandler en,
    PinHandler scl,
    PinHandler sda,
    PinHandler bg, // background led
    PinHandler led) {
    memset(&gKey, 0, sizeof(gKey));

    gKey.vcc = vcc;
    gKey.gnd = gnd;
    gKey.irq = irq;
    gKey.en = en;
    gKey.scl = scl;
    gKey.sda = sda;
    gKey.bg = bg;
    gKey.led = led;

    configPinStatus(gKey.vcc, PIN_OUT);
    configPinStatus(gKey.gnd, PIN_OUT);
    configPinStatus(gKey.en, PIN_OUT);
    initI2c(gKey.scl, gKey.sda);
    configPinStatus(gKey.bg, PIN_OUT);
    configPinStatus(gKey.led, PIN_OUT);
    configPinStatus(gKey.irq, PIN_IN);
    registerPinProcess(gKey.irq, IRQ_UP, keypadIrqHandler, NULL);

    powerOnKeypad();
    return 0;
}

int registerKeyProcess(KeyProcess proc, void* context) {
    if (NULL == proc || gKey.vcc == 0 || gKey.proc != NULL) return -1;
    gKey.proc = proc;
    gKey.context = context;
    return 0;
}
int unregisterKeyProcess() {
    gKey.proc = NULL;
    gKey.context = NULL;
    return 0;
}
