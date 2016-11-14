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
#include "./inc/keypad.h"

#if 0
/* define macro to init uart */
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
      UTCTL##num |= SSEL0; \
      UBR0##num = 0x03; \
      UBR1##num = 0x00; \
      UMCTL##num = 0x4A; \
      UCTL##num &= ~SWRST; \
    } \
  }
 

    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
#endif
    
void InitMsp430() {
  /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
  
    WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    
    P6DIR |= BIT2;P6OUT |= BIT2;    //�رյ�ƽת��
    CCTL0 = CCIE;                   //ʹ��CCR0�ж�
    CCR0 = 2047;                    //�趨����0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //��ʱ��A��ʱ��Դѡ��ACLK��������ģʽ
}
    
/****************������****************/
void main(void)
{  
      uchar tmp;
      int i = 200;
      int j  = 200;
      uchar *ptmp = "shanjiejing test uart.";
      uchar *p = ptmp;
      uchar buffer[32] = "\0";
    InitMsp430();
    P2DIR = 0xff;                   //����P2�ڷ���Ϊ���
    P2OUT = 0xff;
    
    //init uart0&uart1
    initUart();
    openUart(0);
    openUart(1);
 
    _EINT();                        //ʹ��ȫ���ж�
    //LPM3;                           //CPU����LPM3ģʽ
    /* read from uart1 and write to uart0*/
    //Init_Keypad();

    while(1) {
      //Key_Event();
      
      //if (key_Flag == 1) {
        //key_Flag = 0;
        //writeTo0Str(p, 22);
        LPM4;
        j = readFrom(0, buffer, 32);
        for (i = 0; i < j; i ++)
          writeTo(0, buffer[i]);


        //while(1);
        /*for (p = ptmp; *p != '\0'; p++)
          writeTo(0, *p);
        writeTo(0, '\n');*/
      //}
      //if (!IFG2&URXIFG1) continue;
      /*tmp = readFrom(1);
      //while(!IFG1&UTXIFG0);
      //TXBUF0 = tmp;
      writeTo(0, tmp);
      //delay
      while (i--) {
        j = 200;
        while(j--);
      }*/
      //LPM3;
    }
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
}*/