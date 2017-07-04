/*
* lock.c - implement lock module.
*
* Author: Shan Jiejing
* Date: 2017-06-05
*/

#include <stdio.h>
#include <string.h>

#include "lock.h"
#include "bluetoothlock.h"


static Lock gLock;
static LockConfig gDefaultConfig ;

int initLock(LockConfig* config) {
    memset(&gDefaultConfig, 0, sizeof(gDefaultConfig));
    gDefaultConfig.isFirstBoot = 1;
    memcpy(gDefaultConfig.key, "123456", 6);
    memcpy(gDefaultConfig.bluetoothPin, "123456", 6);
    memcpy(gDefaultConfig.btName, "autolife", 8);
    
    memset(&gLock, 0, sizeof(gLock));
    if (NULL == config || 0 != config->isFirstBoot) gLock.config = gDefaultConfig;
    else gLock.config = *config;
    gLock.status = STATUS_LOCK;
    if (0 != gLock.config.isFirstBoot) gLock.mode = CONFIG_MODE;
    else gLock.mode = NORMAL;
    gLock.config.isFirstBoot = 0;
    updateConfig(&gLock.config);

    //TODO:init bluetooth
    initBluetoothLock(&gLock);
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

char isConfigMode() {
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