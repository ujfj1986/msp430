/*
* i2c.c - implement the i2c module.
*
* Author: Shan Jiejing
* Date: 2017-02-06
*/

#include "i2c.h"
#include <string.h>
#include <msp430f149.h> 
#include "pin_interface.h"

typedef struct I2cBus {
//    PinHandler scl;
    //PinHandler sda;
    char rbuf;
} I2cBus;

static I2cBus i2c;

#define SCL_UP setPinValue(I2C_SCL, 1)
#define SCL_DOWN setPinValue(I2C_SCL, 0)
#define SDA_UP setPinValue(I2C_SDA, 1)
#define SDA_DOWN setPinValue(I2C_SDA, 0)
#define I2C_DELAY(n) __delay_cycles(n)

static void i2cInit() {
    configPinStatus(I2C_SCL, PIN_OUT);
    configPinStatus(I2C_SDA, PIN_OUT);

    SCL_UP;
    SDA_UP;
}

//int initI2c(PinHandler scl, PinHandler sda) {
int initI2c() {
    memset(&i2c, 0, sizeof(i2c));

    //i2c.scl = scl;
    //i2c.sda = sda;

    i2cInit();
    return 0;
}

static void i2cStart() {
    i2cInit();

    SDA_DOWN;
    I2C_DELAY(5);
    SCL_DOWN;
}

static void i2cStop() {
    configPinStatus(I2C_SDA, PIN_OUT);
    SCL_UP;
    I2C_DELAY(5);
    SDA_UP;
}

static char testSDA() {
    configPinStatus(I2C_SDA, PIN_IN);
    return getPinValue(I2C_SDA);
}

//write char return ack, don't send start&stop signal
static char i2cWriteChar(char v) {
    int i = 7;
    char ack = 0x00;
    configPinStatus(I2C_SDA, PIN_OUT);
    for (i = 7; i >= 0; i --) {
        SCL_DOWN;
        I2C_DELAY(5);
        if (0 != (v & (0x1 << i))) {
            SDA_UP;
        } else {
            SDA_DOWN;
        }
        I2C_DELAY(5);
        SCL_UP;
        I2C_DELAY(10);
    }
    SCL_DOWN;
    I2C_DELAY(10);
    SCL_UP;
    I2C_DELAY(5);
    ack = testSDA();
    I2C_DELAY(5);
    return ack;
}

static void writeACK() {
    configPinStatus(I2C_SDA, PIN_OUT);
    SDA_UP;
}
//read char return char, don't send start&stop signal
static char i2cReadChar() {
    int i = 7;
    char v = 0x00;
    char t = 0x00;

    configPinStatus(I2C_SDA, PIN_IN);
    for (i = 7; i >= 0; i --) {
        SCL_DOWN;
        I2C_DELAY(10);
        SCL_UP;
        I2C_DELAY(5);
        t = getPinValue(I2C_SDA);
        v |= (t << i);
        I2C_DELAY(5);
    }
    SCL_DOWN;
    I2C_DELAY(5);
    writeACK();
    I2C_DELAY(5);
    SCL_UP;
    I2C_DELAY(10);
    return v;
}

int i2cWrite(char addr, char reg, char value) {
    char ack = 0x00;
    int ret = 0;

    i2cStart();
    ack = i2cWriteChar(addr);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
    ack = i2cWriteChar(reg);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
    ack = i2cWriteChar(value);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
error:
    i2cStop();
    return ret;
}

int i2cRead(char addr, char reg, char* value) {
    char ack = 0x00;
    int ret = 0;

    i2cStart();
    ack = i2cWriteChar(addr);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
    ack = i2cWriteChar(reg);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
    i2cStop();
    i2cStart();
    ack = i2cWriteChar(addr | 0x01);
    if (0 == ack) {
        ret = -1;
        goto error;
    }
    *value = i2cReadChar();
error:
    i2cStop();
    return ret;
}