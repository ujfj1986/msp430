/***************************************************
* main.c - main loop
*
* Author: Shan Jiejing
* Date: 2016.11.15
***************************************************/
#include  <msp430x14x.h>
#include <stdio.h>
#include "./inc/include.h"
#include "./inc/uart.h"
#include "./inc/timer.h"
#include "./inc/event.h"
#include "./inc/log.h"
#include "./inc/pin.h"
#include "./inc/bluetoothdevice.h"
#include "./inc/bluetoothlock.h"
#include "./inc/config.h"
#include "./inc/lock.h"
    
void InitMsp430() {
  /*�������г����ر����е�IO��*/
    // P1DIR = 0XFF;P1OUT = 0XFF;
    // P2DIR = 0XFF;P2OUT = 0XFF;
    // P3DIR = 0XFF;P3OUT = 0XFF;
    // P4DIR = 0XFF;P4OUT = 0XFF;
    // P5DIR = 0XFF;P5OUT = 0XFF;
    // P6DIR = 0XFF;P6OUT = 0XFF;
    initPins();
  
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    initTimer();
    //init uart0&uart1
    initUart();
    initEvent();
    initLog(UART0);
/*
    P6DIR |= BIT2;P6OUT |= BIT2;    //�رյ�ƽת��

#endif*/
}

void readuart0(void* context) {
  unsigned char count = 0;
  unsigned char buf[10] = "\0";
  readStrFrom(UART0, buf, 10);
  count ++;
  //P2OUT = ~count;
  //writeStrTo(UART0, buf, 10);
  log("%s\n", buf);
}
/****************������****************/
void main(void)
{
    InitMsp430();
    // load config
    LockConfig config;
    loadConfig(&config);
    //init global
    initLock(&config);
    
    registerEventProcess(UART0READ, readuart0, NULL);
    openUart(UART0);
    //openUart(1);
 
    _EINT();                        //ʹ��ȫ���ж�
    //LPM3;                           //CPU����LPM3ģʽ
    /* read from uart1 and write to uart0*/
    while (1) {
        //_BIS_SR(LPM3_bits + GIE);
        /*SUSPEND();

        readByteFrom(UART0, buf);
        count ++;
        P2OUT = ~count;
        writeByteTo(UART0, buf[0]);*/
      processEvents();
    }
        
        

}


