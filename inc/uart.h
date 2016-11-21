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


int initUart();
int openUart(int num);
int readStrFrom(int num, unsigned char *pbuffer, int size);
int readByteFrom(int num, unsigned char *pbuffer);
int writeByteTo(int num, unsigned char);
int writeStrTo(int num, unsigned char*, int);
int closeUart(int num);
#pragma vector = UART1RX_VECTOR
__interrupt void Uart1_Rx(void);
#pragma vector = UART0RX_VECTOR
__interrupt void Uart0_Rx(void);

#endif //_SERIAL_H_