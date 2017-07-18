/*
* keypadlock.c - implement keypad lock.
*
* Author: Shan Jiejing
* Date: 2017-07-17
*/

#include "keypadlock.h"
#include "keypad.h"
#include "lock.h"
#include "log.h"
#include <string.h>

void keypadProc(char* key, void* context) {
    (void*)context;
    log("input key: %s\n", key);

    LockConfig* config = getLockConfig();
    if (isConfigMode()) {
        memcpy(config->key, key, KEY_LEN - 1);
        updateConfig(config);
        return ;
    }

    if (0 == memcmp(key, config->key, KEY_LEN - 1)) {
        unlock();
    }
}

int initKeypadLock() {
    initKeypad();
    registerKeyProcess(keypadProc, NULL);
    return 0;
}