/*
* pin.h - define the pin control interfaces.
*
* The pin control interfaces can control the pins. 
*
* Author: Shan Jiejing
* Date: 2017-01-20
*/
#ifndef _PIN_H_
#define _PIN_H_

typedef enum PinStatus {
    PIN_IN = 0,
    PIN_OUT = 1,
    PIN_SEL = 2,
} PinStatus;

typedef char PinHandler;

PinHandler createPinHandler(char pinsId, char pinId) ;

char getPinsId(PinHandler);

char getPinId(PinHandler);

int initPins();

char getPinValue(PinHandler);

char getPinsValue(char pinsId);

int setPinValue(PinHandler, char value);

int setPinsValue(char pinsId, char value);

/*
int configPinsOut(char pinsId);

int configPinOut(PinHandler);

int configPinSel(PinHandler);

int configPinsIn(char pinsId);

int configPinIn(PinHandler);
*/
int configPinsStatus(char pinsId, PinStatus status);

int configPinStatus(PinHandler pin, PinStatus status);

typedef int (*PinProc)(PinHandler, void*);
int registerPinProc(PinHandler, char, PinProc, void*);

int unregisterPinProc(PinHandler);

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void);

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void);

#endif //_PIN_H_