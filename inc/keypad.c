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
#include "timer.h"
#include "config.h"

static int KEY_TIME_OUT = 10*1000; //1s

typedef struct Keypad {
    PinHandler vcc;
    PinHandler gnd;
    PinHandler irq;
    PinHandler en;
    PinHandler scl;
    PinHandler sda;
    PinHandler bg;
    PinHandler led;
    char key[KEY_LEN];
    char keyCount;
    void* timeoutAlarm;
    KeyProcess proc;
    void* context;
} Keypad;

static Keypad gKey;
typedef struct KeyMappingTable {
    char channel;
    char value;
} KeyMappingTable;
static KeyMappingTable gTable[] = {
    {1, '*'},
    {2, '7'},
    {3, '4'},
    {4, '1'},
    {5, '2'},
    {6, '5'},
    {7, '3'},
    {8, '6'},
    {9, '9'},
    {10, '#'},
    {11, '0'},
    {12, '8'},
};

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

static char mappingKey(char* buf) {
    int i = 0;
    int j = 0;

    for (i = 0; i < 3; i++) {
        if (0 == buf[i]) continue;
        for (j = 0; j < 4; j++) {
            if (0 != (buf[i] & (0x11 << (2 * j)))) break;
        }
    }
    if (i == 3) return -1;

    return gTable[i*4 + j].value;
}

static void keyInputTimeout(void* context) {
    gKey.keyCount = 0;
}

static int keypadIrqHandler(PinHandler irq, void* context) {
    //dual with keypad irq event
    char output[3] = "\0";
    int ret = 0;
    char v = 0;

    setPinValue(gKey.en, 0);
    //TODO: verify the registers of output.
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT1, &output[0]);
    if (ret) goto error;
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT2, &output[1]);
    if (ret) goto error;
    ret = i2cRead(KEYPAD_I2C_ADDR, KEYPAD_I2C_REG_OUTPUT3, &output[2]);
    if (ret) goto error;

    //mapping the output to the key.
    v = mappingKey(output);
    if (-1 == v) goto error;
    //TODO: call the call back function to dual with the key event.
    gKey.key[gKey.keyCount++] = v;
    if (gKey.keyCount == KEY_LEN) {
        gKey.proc(gKey.key, gKey.context);
        gKey.keyCount = 0;
        // remove input timeout alarm.
        removeAlarm(gKey.timeoutAlarm);
    } else {
        if (NULL != gKey.timeoutAlarm) removeAlarm(gKey.timeoutAlarm);
        gKey.timeoutAlarm = setAlarm(KEY_TIME_OUT, keyInputTimeout, NULL);
    }

    //return 0;
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


