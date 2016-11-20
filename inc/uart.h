/*
 * serial.h - define the serial interface
 *
 * Author: shan jiejing
 * Date: 2016-11-3
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <msp430x14x.h>
#include "include.h"
#include "buffer.h"

#define UART0 0
#define UART1 1

struct Uart {
  int index;
  struct Buffer rBuffer;
  struct Buffer wBuffer;
};

//struct Uart pUart[2];

int initUart();
int openUart(int num);
int readFrom(int num, unsigned char *pbuffer, int size);
int writeByteTo(int num, unsigned char);
int writeStrTo(int num, unsigned char*, int);
int closeUart(int num);

#endif //_SERIAL_H_