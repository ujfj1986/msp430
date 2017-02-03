/*
* event.c - impletement the events interface.
*
* uart or timer call raiseEvent in the irq function.
* raiseEvent will wake the cpu and record the event type.
* Cpu will call processEvents to dual with events utill there are no events.
* If there are no events, Cpu will entry suspend status.
* Other mode can register a event process function. When the event raise,
* the function will be call.
*
* Author: Shan Jiejing
* Date: 2016-12-25
*/

#include "event.h"
#include "pm.h"
#include <string.h>
#include <stdio.h>
#include "log.h"

typedef struct Events {
  EventType e;
  EventProcess proc;
  void* context;
} Events;
#define MAX_EVENT_TYPE 32
static Events allEvents[MAX_EVENT_TYPE];
extern volatile int EVENTSTATUS = 0;

int initEvent() {
  memset(&allEvents, 0, sizeof(allEvents));
  /*allEvents[UART0READ].e = UART0READ;
  allEvents[UART1READ].e = UART1READ;
  allEvents[ALARM].e = ALARM;*/
  int i = 0;
  for (i = 0; i < MAX_EVENT; i++) {
    allEvents[i].e = i;
  }
  EVENTSTATUS = 0;
  return 0;
}

void processEvents() {
  if (0 == EVENTSTATUS) SUSPEND();
  do {
    int i = 0;
    for (i = 0; i < MAX_EVENT; i++) {
      if (0 != (EVENTSTATUS & (1 << i))) {
        if (NULL != allEvents[i].proc) {
          allEvents[i].proc(allEvents[i].context);
        }
        EVENTSTATUS &= ~(1 << i);
      }
    }
  } while(0 != EVENTSTATUS);

  // if (0 != (EVENTSTATUS & (1 << UART0READ))) {
  //   if (NULL != allEvents[UART0READ].proc) {
  //     allEvents[UART0READ].proc(allEvents[UART0READ].context);
  //   }
  //   EVENTSTATUS &= ~(1 << UART0READ);
  // } else if (0 != (EVENTSTATUS & (1 << UART1READ))) {
  //   if (NULL != allEvents[UART1READ].proc) {
  //     allEvents[UART0READ].proc(allEvents[UART1READ].context);
  //   }
  //   EVENTSTATUS &= ~(1 << UART1READ);
  // } else if (0 != (EVENTSTATUS & (1 << ALARM))) {
  //   if (NULL != allEvents[ALARM].proc) {
  //     allEvents[ALARM].proc(allEvents[ALARM].context);
  //   }
  //   EVENTSTATUS &= ~(1 << ALARM);
  // }
}

int registerEventProcess(EventType e, EventProcess proc, void* context) {
  if (NULL != allEvents[e].proc) return -1;
  allEvents[e].proc = proc;
  allEvents[e].context = context;
  return 0;
}

int unregisterEventProcess(EventType e) {
  if (NULL == allEvents[e].proc) return 0;
  allEvents[e].proc = NULL;
  allEvents[e].context = NULL;
  return 0;
}
  