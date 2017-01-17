/*
* event.h - define events interface.
*
* define the events which are received by the core.
* And define the struct of dual with them.
*
* Author: Shan Jiejing.
* date: 2016-12-25
*/
#ifndef _EVENT_H_
#define _EVENT_H_

#include "pm.h"

extern volatile int EVENTSTATUS;
typedef enum EventType {
  UART0READ = 0,
  UART1READ = 1,
  ALARM     = 2,
  BLUETOOTH_CONNECT = 3,
  BLUETOOTH_READ = 4,
  BLUETOOTH_DISCONNECT = 5,
} EventType;

int initEvent();

// register a event process function.
typedef void (*EventProcess)(void*);
int registerEventProcess(EventType e, EventProcess proc, void* context);

int unregisterEventProcess(EventType e);

// raise a event and trigger core to call processEvents to dual with it.
//void raiseEvent(EventType e);
//process events utill there are no events.
void processEvents();

#define raiseEvent(e) do{ \
  EVENTSTATUS |= (1 << e); \
  RESUME(); \
} while(0)

#endif //_EVENT_H_