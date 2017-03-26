/*
* store.h - define the interfaces of store module.
*
* store module initializes the msp149 flash segment A,
* and read/write config structure.
*
* Author: Shan Jiejing
* Date: 20170326
*/
#ifndef _STORE_H_

#define _STORE_H_

#include "config.h"
int initStore();
int readConfig(LockConfig* config);
int writeConfig(LockConfig* config);


#endif //_STORE_H_