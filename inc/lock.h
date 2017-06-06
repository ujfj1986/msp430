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
    STATUS_LOCK = 0,
    STATUS_UNLOCK = 1,
} LockStatus;
LockStatus getLockStatus();

typedef enum LockMode {
    NORMAL = 0,
    CONFIG_MODE = 1,
} LockMode;
char isConfigMode();

typedef struct Lock {
    LockConfig config;
    LockStatus status;
    LockMode mode;
} Lock;

LockConfig* getLockConfig();

int lock();
int unlock();

#endif //_LOCK_H_