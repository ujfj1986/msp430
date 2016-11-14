/*
 * uart.c - implement the uart module.
 *
* Author: Shan Jiejing
* Date: 2016-11-3
*/

#include "uart.h"

//#define NULL ((void *)0)

struct Uart gUart[2];

int initUart() {
  gUart[UART0].index = UART0;
  initBuffer(&(gUart[UART0].rBuffer));
  initBuffer(&(gUart[UART0].wBuffer));
  gUart[UART1].index = UART1;
  initBuffer(&(gUart[UART1].rBuffer));
  initBuffer(&(gUart[UART1].wBuffer));
  return 0;
}

#if 0
#define InitUart(num) \
  { \
    if (num == 0 || num ==1) { \
      if (num == 0) { \
        P3SEL |= 0x30; \
        ME1 |= URXE0 + UTXE0; \
        IE1 |= URXIE0 + UTXIE0; \
        UCTL0 |= CHAR; \
      } else { \
        P3SEL |= 0xC0; \
        ME2 |= URXE1 + UTXE1; \
        IE2 |= URXIE1 + UTXIE1; \
      } \
      UTCTL#num |= SSEL0; \
      UBR0#num = 0x03; \
      UBR1#num = 0x00; \
      UMCTL#num = 0x4A; \
      UCTL#num &= ~SWRST; \
    } \
  }
#endif

int openUart(int num) {
  if (num != 0 && num != 1) return -1;
  if (num == 0) { 
    UCTL0 |= SWRST;
    P3SEL |= 0x30; 
    ME1 |= URXE0 + UTXE0; 
    IE1 |= URXIE0 + UTXIE0; 
    UCTL0 |= CHAR; 
    UTCTL0 |= SSEL0; 
    UBR00 = 0x03; 
    UBR10 = 0x00; 
    UMCTL0 = 0x4A; 
    UCTL0 &= ~SWRST;
    U0TXBUF = 0x00;
  } else { 
    UCTL1 |= SWRST;
    P3SEL |= 0xC0; 
    ME2 |= URXE1 + UTXE1; 
    IE2 |= URXIE1 + UTXIE1; 
    UTCTL1 |= SSEL0; 
    UBR01 = 0x03; 
    UBR11 = 0x00; 
    UMCTL1 = 0x4A; 
    UCTL1 &= ~SWRST;
    U1TXBUF = 0x00;
  } 

  return 0;
}

#pragma vector = UART1RX_VECTOR
__interrupt void Uart1_Rx(void) {
  struct Buffer *pBuffer = &(gUart[UART1].rBuffer);
  unsigned char tmp = U1RXBUF;
  writeBuffer(pBuffer, tmp);
  LPM4_EXIT;
  //trigger uart1 rx event
}

#pragma vector = UART0RX_VECTOR
__interrupt void Uart0_Rx(void) {
  struct Buffer *pBuffer = &(gUart[UART0].rBuffer);
  unsigned char tmp = U0RXBUF;
  writeBuffer(pBuffer, tmp);
  LPM4_EXIT;
  //trigger uart0 rx event
}

int readFrom(int num, unsigned char* pVal, int size) {
  struct Buffer *pBuffer = NULL;
  if (num != 0 && num != 1) return -1;
  
  pBuffer = &(gUart[num].rBuffer);
  return readBuffer(pBuffer, pVal, size);
}

int writeTo(int num, unsigned char val) {
  if (num != 0 && num != 1) return -1;
  
  //while((num == 0) ? (IFG1 & UTXIFG0) : (IFG2 & UTXIFG1)) ;
  
  if (num == 0) {
    U0TXBUF = val;
    //IFG1 |= UTXIFG0;
    //while( !U0TCTL & TXEPT);
  } else {
    U1TXBUF = val;
    //IFG2 |= UTXIFG1;
    //while( !U1TCTL & TXEPT);
  }
  while((num == 0) ? ((IFG1 & UTXIFG0) == 0) : ((IFG2 & UTXIFG1) == 0));
  //(num == 0) ? (IFG1 &= ~UTXIFG0) : (IFG2 &= ~UTXIFG1);
  if (num == 0) {
    IFG1 &= ~UTXIFG0;
  } else {
    IFG2 &= ~UTXIFG1;
  }
  
  return 0;
}
#if 0
int writeTo0Str(unsigned char* p, int len) {
  unsigned char * t = p;
  int i = 0;
  int j = 0;
  int count = 0;
  if (NULL == p || len <= 0) return -1;
  
  for (j = 0; j <= len / 16; j ++) {
  for (i = 0; i < 16; i++) {
    U0TXBUF = t[j * 16 + i];
    while (((IFG1 & UTXIFG0) == 0) && (count++ <= 1000)) ;
    IFG1 &= ~UTXIFG0;
    //while (!IFG1 & UTXIFG0);
    //U0TXBUF = 0xff;
  }
  }
  j = len / 16;
  for (i = j * 16; i < len; i++) {
    U0TXBUF = t[i];
    while((IFG1 & UTXIFG0) == 0);
    IFG1 &= ~UTXIFG0;
  }

  U0TXBUF = '\n';
  while( (IFG1 & UTXIFG0) == 0);
  IFG1 &= ~UTXIFG0;
  
  return 0;
}
#else
int writeTo0Str(unsigned char* p, int len) {
  unsigned char * t = p;
  int i = 0;
  int j = 0;
  int count = 0;
  if (NULL == p || len <= 0) return -1;
  
  //for (j = 0; j <= len / 16; j ++) {
  for (i = 0; i < len; i++) {
    U0TXBUF = t[ i];
    while (((IFG1 & UTXIFG0) == 0) && (count++ <= 1000)) ;
    IFG1 &= ~UTXIFG0;
    //while (!IFG1 & UTXIFG0);
    //U0TXBUF = 0xff;
  }
  /*}
  j = len / 16;
  for (i = j * 16; i < len; i++) {
    U0TXBUF = t[i];
    while((IFG1 & UTXIFG0) == 0);
    IFG1 &= ~UTXIFG0;
  }*/

  U0TXBUF = '\n';
  while( (IFG1 & UTXIFG0) == 0);
  IFG1 &= ~UTXIFG0;
  
  return 0;
}
#endif

int closeUart(int num) {
  if (num != 0 && num != 1) return -1;
  
  if (num == 0) {
    while((IFG1 & UTXIFG0) != 0);
    P3SEL &= 0xCF;
    P3DIR |= 0x30;
    /*ME1 = 0;
    IE1 = 0;
    UTCTL0 = 0;*/
  } else {
    while((IFG2 & UTXIFG1) != 0);
    P3SEL &= 0x3F;
    P3DIR |= 0xC0;
    /*ME2 = 0;
    IE2 = 0;
    UTCTL1 = 0;*/
  }

  return 0;
}