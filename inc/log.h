/*
* log.h - define the log interface.
*
* the log module uses the uart to print the log.
* When this module init, user will input a uart number,
* and then, if user call log method to print string, it will
* output the string to the uart.
*
* Author: Shan Jiejing
* Date: 2017-1-1
*/
#ifndef _LOG_H_
#define _LOG_H_

int initLog(int uartnu);

int log(const char* format, ...);

#endif //_LOG_H_