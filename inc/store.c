/*
* store.c - implement the store module.
*
* Author: Shan Jiejing
* Date: 2017-03-26
*/

#include <stdio.h>
#include <msp430x14x.h>
#include "store.h"

#define STORE_ADDR 0x1000

#define SEG_SIZE  128

int initStore() {
    //set the clock
    FCTL2 = FWKEY + FSSEL0 + FN0;
    return 0;
}

int readStore(int offset, char* buf, int len) {
    if (0 > offset || NULL == buf || 0 >= len) return -1;

    FCTL1 = FRKEY;
    FCTL3 = FRKEY;
    int i = 0;
    unsigned char* p = (unsigned char*)STORE_ADDR;
    for (i = 0; i < len; i++) {
        *(buf + i) = *(p + offset + i);
    }
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
    return 0;
}
int writeStore(int offset, char* buf, int len) {
    if (0 > offset || NULL == buf || 0 >= len) return -1;
    if (0 != offset) return -1;
    if (SEG_SIZE < (offset + len)) return -1;
    unsigned char* p = (unsigned char* )STORE_ADDR;
    int i = 0;

    //erase segment
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;
    *p = 0;
    FCTL1 = FWKEY + WRT;
    for(i = 0; i < len; i ++) {
        *(p + i) = buf[i];
    }
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
    return 0;
}