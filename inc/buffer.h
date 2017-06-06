/*
 * buffer.h - define a buffer struct.
 *
 * this buffer struct is a circular buffer.
 *
* Author: Shan Jiejing
* Date: 2016.11.15
*/

#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFF_SIZE 32

typedef struct Buffer {
  unsigned char pBuffer[BUFF_SIZE];
  unsigned char head;
  unsigned char end;
  unsigned char full;
} Buffer;

int initBuffer(struct Buffer *pBuffer);
int writeBuffer(struct Buffer* pBuffer, unsigned char val);
int readBuffer(struct Buffer* pBuffer, unsigned char* pVal, int size);

#endif
