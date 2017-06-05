/*
* lock.c - implement lock module.
*
* Author: Shan Jiejing
* Date: 2017-06-05
*/

#include <stdio.h>
#include <string.h>

#include "lock.h"

typedef struct Lock {
    LockConfig config;
    LockStatus status;
    LockMode mode;
} Lock;

static Lock gLock;
static LockConfig gDefaultConfig = {};//TODO

int initLock(LockConfig* config) {
    if (NULL == config) return -1;

    memset(&gLock, 0, sizeof(gLock));
    gLock.config = *config;
    gLock.status = LOCK;
    if (0 != gLock.config.isFirstBoot) gLock.mode = NORMAL;
    else gLock.mode = CONFIG_MODE;

    //TODO:init bluetooth
    //init keypad
    // init motor
    // init beep
    // register reset pin process
    return 0;
}

LockStatus getLockStatus() {
    return gLock.status;
}

LockConfig* getLockConfig() {
    return &(gLock.config);
}

bool isConfigMode() {
    return (CONFIG_MODE == gLock.mode);
}

int lock() {
    //TODO: lock
    return 0;
}

int unlock() {
    //TODO: unlock
    return 0;
}