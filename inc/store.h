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

int initStore();
int readStore(int offset, char* buf, int len);
int writeStore(int offset, char* buf, int len);


#endif //_STORE_H_