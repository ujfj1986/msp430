/*
* config.c - implement the config load and store function.
*
* Author: Shan Jiejing
* Date: 2017-06-03
*/

#include <stdio.h>

#include "config.h"
#include "store.h"


int loadConfig(LockConfig* config) {
    if (NULL == config) return -1;

    return readStore(0, (char*)config, sizeof(LockConfig));
}
int updateConfig(LockConfig* config) {
    if (NULL == config) return -1;

    return writeStore(0, (char*)config, sizeof(LockConfig));
}