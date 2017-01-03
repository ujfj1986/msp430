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
    
void InitMsp430() {
  /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
  
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
/*
    P6DIR |= BIT2;P6OUT |= BIT2;    //关闭电平转换

#endif*/
}

void readuart0(void* context) {
  unsigned char count = 0;
  unsigned char buf[10] = "\0";
  readStrFrom(UART0, buf, 10);
  count ++;
  P2OUT = ~count;
  //writeStrTo(UART0, buf, 10);
  log("%s\n", buf);
}
/****************主函数****************/
void main(void)
{
    //unsigned char buf[10] = "\0";
    //unsigned char count = 0;
    InitMsp430();
    initTimer();
    
    //init uart0&uart1
    initUart();
    initEvent();
    initLog(UART0);

    registerEventProcess(UART0READ, readuart0, NULL);
    openUart(UART0);
    //openUart(1);
 
    _EINT();                        //使能全局中断
    //LPM3;                           //CPU进入LPM3模式
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


