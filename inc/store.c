/*
* store.c - implement the store module.
*
* Author: Shan Jiejing
* Date: 2017-03-26
*/

#include <msp430x14x.h>
#include "store.h"

#define STORE_ADDR 0x1000

int initStore() {
    //set the fctl2
    FCTL2 = FWKEY + FSSEL0 + FN0;
    return 0;
}

int readConfig(LockConfig* config) {
    
}
int writeConfig(LockConfig* config) {

}