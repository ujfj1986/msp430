/*
* timer.h - define timer interface.
*
* Author: Shanjiejing
* Date: 2016-11-24
*/
#ifndef _TIMER_H_
#define _TIMER_H_

int initTimer();
unsigned long long getCurrentTime();
void delay_ms(unsigned int ms);
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void);
/* TODO - impletment Alarm function.
typedef void (*alarmCallback)(void *context);
void setAlarm(unsigned int ms, alarmCallback callback, void* context);*/

#endif