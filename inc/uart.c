/*
 * uart.c - implement the uart module.
 *
* Author: Shan Jiejing
* Date: 2016-11-3
*/

#include <msp430x14x.h>
#include "uart.h"
#include "include.h"
#include "buffer.h"
#include "timer.h"
#include "pm.h"
#include "event.h"

struct Uart {
  int index;
  struct Buffer rBuffer;
  int status;
};

//#define NULL ((void *)0)

struct Uart gUart[2];
#define CLOSED 0
#define OPENED 1

int initUart() {
  gUart[UART0].index = UART0;
  initBuffer(&(gUart[UART0].rBuffer));
  gUart[UART0].status = CLOSED;
  gUart[UART1].index = UART1;
  initBuffer(&(gUart[UART1].rBuffer));
  gUart[UART1].status = CLOSED;
  return 0;
}

int openUart(int num) {
  if (num != 0 && num != 1) return -1;
  if (OPENED == gUart[num].status) return 0;
  if (num == 0) { 
    P3SEL |= 0x30;
    ME1 |= URXE0 + UTXE0;
    UCTL0 |= CHAR; 
    UTCTL0 |= SSEL0; 
    UBR00 = 0x03; 
    UBR10 = 0x00; 
    UMCTL0 = 0x4A; 
    UCTL0 &= ~SWRST;
    IE1 |= URXIE0;
    gUart[UART0].status = OPENED;
  } else { 
    P3SEL |= 0xC0; 
    ME2 |= URXE1 + UTXE1;
    UCTL1 |= CHAR;
    UTCTL1 |= SSEL0; 
    UBR01 = 0x03; 
    UBR11 = 0x00; 
    UMCTL1 = 0x4A; 
    UCTL1 &= ~SWRST; 
    IE2 |= URXIE1;
    gUart[UART1].status = OPENED;
  } 

  return 0;
}

#pragma vector = UART1RX_VECTOR
__interrupt void Uart1_Rx(void) {
  struct Buffer *pBuffer = &(gUart[UART1].rBuffer);
  unsigned char tmp = U1RXBUF;
  writeBuffer(pBuffer, tmp);
  //_BIC_SR_IRQ(LPM3_bits);
  //RESUME();
  //trigger uart1 rx event
  raiseEvent(UART1READ);
}

#pragma vector = UART0RX_VECTOR
__interrupt void Uart0_Rx(void) {
  struct Buffer *pBuffer = &(gUart[UART0].rBuffer);
  unsigned char tmp = U0RXBUF;
  writeBuffer(pBuffer, tmp);
 //_BIC_SR_IRQ(LPM3_bits);
  //RESUME();
  //trigger uart0 rx event
  raiseEvent(UART0READ);
}

int readByteFrom(int num, unsigned char* pVal) {
  struct Buffer *pBuffer = NULL;
  if (num != 0 && num != 1) return -1;
  if (CLOSED == gUart[num].status) return -2;
  pBuffer = &(gUart[num].rBuffer);
  return readBuffer(pBuffer, pVal, 1);
}

int readStrFrom(int num, unsigned char* pVal, int size) {
  struct Buffer *pBuffer = NULL;
  if (num != 0 && num != 1) return -1;
  if (CLOSED == gUart[num].status) return -2;
  
  //-TODO: sleep 500ms, and then return buffer.
  delay_ms(500);
  pBuffer = &(gUart[num].rBuffer);
  return readBuffer(pBuffer, pVal, size);
}

int writeByteTo(int num, unsigned char val) {
  if (num != 0 && num != 1) return -1;
  if (CLOSED == gUart[num].status) return -2;
  
  if (num == 0) {
    U0TXBUF = val;
  } else {
    U1TXBUF = val;
  }
  while((num == 0) ? ((IFG1 & UTXIFG0) == 0) : ((IFG2 & UTXIFG1) == 0));
  if (num == 0) {
    IFG1 &= ~UTXIFG0;
  } else {
    IFG2 &= ~UTXIFG1;
  }
  
  return 0;
}

int writeStrTo(int num, unsigned char* p, int len) {
  int i = 0;
  if (NULL == p || len <= 0) return -1;
  if (CLOSED == gUart[num].status) return -2;

  for (i = 0; p[i] != '\0' && i < len; i++)
    writeByteTo(num, p[i]);
  
  return i;
}


int closeUart(int num) {
  if (num != 0 && num != 1) return -1;
  if (CLOSED == gUart[num].status) return 0;
  
  if (num == 0) {
    P3SEL &= 0xCF;
    P3DIR |= 0x30;
    gUart[UART0].status = CLOSED;
  } else {
    P3SEL &= 0x3F;
    P3DIR |= 0xC0;
    gUart[UART1].status = CLOSED;
  }

  return 0;
}