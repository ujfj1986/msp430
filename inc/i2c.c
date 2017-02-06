/*
* i2c.c - implement the i2c module.
*
* Author: Shan Jiejing
* Date: 2017-02-06
*/

#include "i2c.h"
#include <string.h>
#include <msp430f149.h> 

typedef struct I2cBus {
    PinHandler scl;
    PinHandler sda;
    char rbuf;
} I2cBus;

static I2cBus i2c;

#define SCL_UP setPinValue(i2c.scl, 1)
#define SCL_DOWN setPinValue(i2c.scl, 0)
#define SDA_UP setPinValue(i2c.sda, 1)
#define SDA_DOWN setPinValue(i2c.sda, 0)
#defien DELAY(n) __delay_cycles(n)

static void i2cInit() {
    configPinStatus(i2c.scl, PIN_OUT);
    configPinStatus(i2c.sda, PIN_OUT);

    SCL_UP;
    SDA_UP;
}

int initI2c(PinHandler scl, PinHandler sda) {
    memset(&i2c, 0, sizeof(i2c));

    i2c.scl = scl;
    i2c.sda = sda;

    i2cInit();
    return 0;
}

static void i2cStart() {
    i2cInit();

    SDA_DOWN;
    DELAY(5);
    SCL_DOWN;
}

static void i2cStop() {
    configPinStatus(i2c.sda, PIN_OUT);
    SCL_UP;
    DELAY(5);
    SDA_UP;
}

static char testSDA() {
    configPinStatus(i2c.sda, PIN_IN);
    return getPinValue(i2c.sda);
}

static char i2cWriteChar(char v) {
    int i = 7;
    configPinStatus(i2c.sda, PIN_OUT);
    for (i = 7; i >= 0; i --) {
        SCL_DOWN;
        DELAY(5);
        if (0 != (v & (0x1 << i))) {
            SDA_UP;
        } else {
            SDA_DOWN;
        }
        DELAY(5);
        SCL_UP;
        DELAY(10);
    }
    return testSDA();
}