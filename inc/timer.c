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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "list.h"
#include "event.h"
#include "log.h"

#define PERIOD  408 //define the timer period, 0.1s
#define TIME_TICK 100 //define the time tick as 100ms
volatile unsigned long long tick = 0;

typedef struct Alarm {
  ListNode next;
  unsigned long long alarmTime;
  alarmCallback callback;
  void* context;
} Alarm;
static List AlarmList;

int initTimer() {
    CCTL0 = CCIE;                   //ʹ��CCR0�ж�
    CCR0 = PERIOD;                    //�趨����0.5S                
    TACTL = TASSEL_1 + ID_3 + MC_1; //��ʱ��A��ʱ��Դѡ��ACLK��������ģʽ
    initList(&AlarmList);
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
�������ƣ�Timer_A 
��    �ܣ���ʱ��A���жϷ�������
��    ������
����ֵ  ����
********************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{  
    tick ++;
    //log("%lld\n", tick);
    //wake the cpu
    //set the event flag
    if (AlarmList.len > 0) {
      Alarm* tmp = (Alarm*)AlarmList.head;
      if (tick >= tmp->alarmTime) {
        raiseEvent(ALARM);
      }
    }
}

static int compareAlarm(ListNode* lNode, ListNode* rNode) {
  if (NULL == lNode) return 1;
  if (NULL == rNode) return 0;

  Alarm* lAlarm = (Alarm*) lNode;
  Alarm* rAlarm = (Alarm*) rNode;

  if (lAlarm->alarmTime > rAlarm->alarmTime) {
    return 1;
  } else if (lAlarm->alarmTime == rAlarm->alarmTime) {
    return 0;
  }
  return -1;
}

static void alarmProccess(void* context) {
  if (NULL == context) return ;

  ListNode* node = delNode(&AlarmList);
  if (NULL == node) {
    return;
  }

  Alarm* tmp = (Alarm*) node;
  if (tmp->alarmTime <= tick) {
    tmp->callback(tmp->context);
  } else {
    insert(&AlarmList, node, compareAlarm);
  }
  if (AlarmList.len == 0) {
    unregisterEventProcess(ALARM);
  }
}
void setAlarm(unsigned int ms, alarmCallback callback, void* context) {
  if ((0 == ms) || (NULL == callback)) return;

  Alarm* tmp = (Alarm*) malloc(sizeof(Alarm));
  if (NULL == tmp) return ;
  memset(tmp, 0, sizeof(Alarm));

  tmp->alarmTime = getCurrentTime() + (ms / TIME_TICK);
  tmp->callback = callback;
  tmp->context = context;
  insert(&AlarmList, (ListNode*)tmp, compareAlarm);
  if (1 == AlarmList.len) {
    //register Alarm event.
    registerEventProcess(ALARM, alarmProccess, &AlarmList);
  }
}