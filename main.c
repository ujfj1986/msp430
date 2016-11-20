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
  /*�������г���ر����е�IO��*/
    /*P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;*/
  
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
/*
    P6DIR |= BIT2;P6OUT |= BIT2;    //�رյ�ƽת��
    CCTL0 = CCIE;                   //ʹ��CCR0�ж�
    CCR0 = 2047;                    //�趨����0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //��ʱ��A��ʱ��Դѡ��ACLK��������ģʽ
#endif*/
}
    
volatile uchar CpuStatus = 1;
volatile uchar tmp = 0xff;
/****************������****************/
void main(void)
{
    InitMsp430();
    
    //init uart0&uart1
    initUart();
    openUart(0);
    //openUart(1);
 
    _EINT();                        //ʹ��ȫ���ж�
    //LPM3;                           //CPU����LPM3ģʽ
    /* read from uart1 and write to uart0*/

        _BIS_SR(LPM3_bits + GIE);

}

/*******************************************
�������ƣ�Timer_A 
��    �ܣ���ʱ��A���жϷ�����
��    ������
����ֵ  ����
********************************************/
/*#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{  
    //P2OUT ^= 0xff;                  //P2�����ȡ��
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