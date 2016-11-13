/*
 * serial.h - define the serial interface
 *
 * Author: shan jiejing
 * Date: 2016-11-3
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <msp430x14x.h>

#define UART0 0
#define UART1 1
#define BUFF_SIZE 32

struct RBuffer {
  unsigned char pBuffer[BUFF_SIZE];
  int head;
  int end;
};

struct WBuffer {
  unsigned char pBuffer[BUFF_SIZE];
  int head;
  int end;
};

struct Uart {
  int index;
  struct RBuffer rBuffer;
  struct WBuffer wBuffer;
};

//struct Uart pUart[2];

int initUart();
int openUart(int num);
int readFrom(int num, unsigned char *pbuffer, int size);
int writeTo(int num, unsigned char);
int writeTo0Str(unsigned char*, int);
int closeUart(int num);

#endif //_SERIAL_H_