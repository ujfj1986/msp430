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
    
void InitMsp430() {
  /*下面六行程序关闭所有的IO口*/
    /*P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;*/
  
    WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
/*
    P6DIR |= BIT2;P6OUT |= BIT2;    //关闭电平转换
    CCTL0 = CCIE;                   //使能CCR0中断
    CCR0 = 2047;                    //设定周期0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //定时器A的时钟源选择ACLK，增计数模式
#endif*/
}
    
volatile uchar CpuStatus = 1;
volatile uchar tmp = 0xff;
/****************主函数****************/
void main(void)
{
    InitMsp430();
    
    //init uart0&uart1
    initUart();
    openUart(0);
    //openUart(1);
 
    _EINT();                        //使能全局中断
    //LPM3;                           //CPU进入LPM3模式
    /* read from uart1 and write to uart0*/

        _BIS_SR(LPM3_bits + GIE);

}

/*******************************************
函数名称：Timer_A 
功    能：定时器A的中断服务函数
参    数：无
返回值  ：无
********************************************/
/*#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{  
    //P2OUT ^= 0xff;                  //P2口输出取反
    //LPM3_EXIT;
}*/
uchar rcount = 0;
#pragma vector = UART0RX_VECTOR
__interrupt void Uart0_Rx(void) {

  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
  //writeTo(0, RXBUF0);
  rcount ++;
  P2OUT = ~rcount;
}