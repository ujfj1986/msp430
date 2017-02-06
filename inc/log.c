/*
* log.c - implement the log module.
*
* Author: Shan Jiejing
* Date: 2017-1-1
*/

#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "uart.h"

#define LOG_BUFFER_SIZE 1000
static int logUartNum = -1;
static unsigned char logBuffer[LOG_BUFFER_SIZE] = "\0";

int initLog(int uartNum) {
  if ( -1 != logUartNum) return -1;
  if (UART0 != uartNum && UART1 != uartNum) return -1;
  logUartNum = uartNum;
  openUart(logUartNum);
  return 0;
}

int log(const char* format, ...) {
  if (-1 == logUartNum) return -1;
  if (NULL == format) return -1;
  
  va_list args;
  int len = 0;
  
  va_start(args, format);
  len = vsprintf((char*)logBuffer, format, args);
  logBuffer[len] = '\0';
  
  if (0 != len) {
    writeStrTo(logUartNum, logBuffer, len+1);
  }
  va_end(args);
  logBuffer[0] = '\0';
  return 0;
}