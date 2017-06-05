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

/*void readSeg(unsigned char* addr, unsigned char* buf) {
    unsigned char* p = addr;
    int i = 0;
    for (i = 0; i < SEG_SIZE; i++) {
        buf[i] = *(p + i);
    }
}*/

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
    if (SEG_SIZE < (offset + len)) return -1;
    unsigned char SEG_BUF[SEG_SIZE] = "\0";
    unsigned char* p = (unsigned char* )STORE_ADDR;
    int i = 0;

    if (0 != readConfig(0, SEG_BUF, SEG_SIZE)) return -1;
    for (i = 0; i < len; i++) {
        *(SEG_BUF + offset + i) = *(buf + i);
    }

    //erase segment
    FCTL1 = FWKEY + ERASE;
    FCTL3 = FWKEY;
    *p = 0;
    FCTL1 = FWKEY + WRT;
    for(i = 0; i < SEG_SIZE; i ++) {
        *(p + i) = SEG_BUF[i];
    }
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
    return 0;
}