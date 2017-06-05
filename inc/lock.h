/*
* lock.h - define global lock struct.
*
* Author: Shan Jiejing
* Date: 2017-06-05
*/

#ifndef _LOCK_H_
#define _LOCK_H_

#include "config.h"

int initLock(LockConfig* config);

typedef enum LockStatus {
    LOCK = 0;
    UNLOCK = 1;
} LockStatus;
LockStatue getLockStatus();

typedef enum LockMode {
    NORMAL = 0,
    CONFIG_MODE = 1,
} LockMode;
bool isConfigMode();

LockConfig* getLockConfig();

int lock();
int unlock();

#endif //_LOCK_H_