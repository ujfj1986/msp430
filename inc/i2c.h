/*
* i2c.h - define the i2c module.
* I2c module uses the gpio pin to simulate I2C bus.
*
* Author: Shan Jiejing
* Date:2017-02-06
*/

#ifndef _I2C_H_
#define _I2C_H_

#include "pin.h"

//int initI2c(PinHandler scl, PinHandler sda);
int initI2c();

int i2cWrite(char addr, char reg, char value);
int i2cRead(char addr, char reg, char* value);

#endif //_I2C_H_