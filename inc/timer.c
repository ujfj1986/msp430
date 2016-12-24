/*
* timer.c - impletment timer function.
*
* 1. enable msp430f149 timer_A
* 2. count every tick
* 3. delay function
* 4. TODO - Alarm function
*
* Author: Shanjiejing
* Date: 2016-11-24
*/

#include <msp430x14x.h>
#include "timer.h"

#define PERIOD  408 //define the timer period, 0.1s
#define TIME_TICK 100 //define the time tick as 100ms
volatile unsigned long long tick = 0;

int initTimer() {
    CCTL0 = CCIE;                   //使能CCR0中断
    CCR0 = PERIOD;                    //设定周期0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //定时器A的时钟源选择ACLK，增计数模式
    return 0;
}
unsigned long long getCurrentTime() {
  return tick;
}
void delay_ms(unsigned int ms) {
  unsigned long long tmp = getCurrentTime();
  
  while((ms / TIME_TICK) < (tick - tmp));
}

/*******************************************
函数名称：Timer_A 
功    能：定时器A的中断服务函数
参    数：无
返回值  ：无
********************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{  
    tick ++;
    //-TODO
    //wake the cpu
    //set the event flag
}