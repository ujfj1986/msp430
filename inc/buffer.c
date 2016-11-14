/*
 * buffer.c - impletement the interface of buffer.
 *
* Author: Shan Jiejing
* Date: 2016.11.15
*/
#include <stdio.h>
#include "buffer.h"

int initBuffer(struct Buffer *pBuffer) {
  if (pBuffer == NULL) return -1;
  pBuffer->head = 0;
  pBuffer->end = 0;
  pBuffer->full = 0;
  return 0;
}
int writeBuffer(struct Buffer* pBuffer, unsigned char val) {
  if (pBuffer == NULL) return -1;
  pBuffer->pBuffer[pBuffer->end++] = val;
  pBuffer->end %= BUFF_SIZE;
  if (pBuffer->full) {
    pBuffer->head++;
    pBuffer->head %= BUFF_SIZE;
  } else if (pBuffer->end == pBuffer->head) {
    pBuffer->full = 1;
  }
  return 0;
}
int readBuffer(struct Buffer* pBuffer, unsigned char* pVal, int size) {
  int len = 0;
  int i = 0;
  if ((pBuffer == NULL) || (pVal == NULL) || (size <= 0)) return -1;
  if (pBuffer->full) {
    len = BUFF_SIZE;
  } else {
    len = (pBuffer->end > pBuffer->head) ?
      (pBuffer->end - pBuffer->head) :
      ((pBuffer->end == pBuffer->head)? 0 : (pBuffer->end + BUFF_SIZE - pBuffer->head));
  }
  if (size < len) {
    len = size;
  }
  for (i = 0; i < len; i++) {
    pVal[i] = pBuffer->pBuffer[((pBuffer->head + i) % BUFF_SIZE)];
  }
  pBuffer->head = (pBuffer->head + len) % BUFF_SIZE;
  pBuffer->full = 0;
  return len;
}