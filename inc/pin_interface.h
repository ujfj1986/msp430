/*
* pin_interface.h - define the msp430 pin interface.
*
* Author: Shan Jiejing
* Date: 2017-07-16
*/
#ifndef _PIN_INTERFACE_H_
#define _PIN_INTERFACE_H_

#include "pin.h"
#include "uart.h"

const PinHandler UART0_TX = createPinHandler(3, 4);
const PinHandler UART0_RX = createPinHandler(3, 5);
const PinHandler UART1_TX = createPinHandler(3, 6);
const PinHandler UART1_RX = createPinHandler(3, 7);

const PinHandler I2C_SCL = createPinHandler(0, 0);
const PinHandler I2C_SDA = createPinHandler(0, 0);

const PinHandler KEY_PAD_VCC = createPinHandler(0, 0);
const PinHandler KEY_PAD_GND = createPinHandler(0, 0);
const PinHandler KEY_PAD_IRQ = createPinHandler(0, 0);
const PinHandler KEY_PAD_EN = createPinHandler(0, 0);
const PinHandler KEY_PAD_BG = createPinHandler(0, 0);
const PinHandler KEY_PAD_LED = createPinHandler(0, 0);

// bluetooth
const PinHandler BT_VCC = createPinHandler(6, 0);
const PinHandler BT_GND = createPinHandler(6, 6);
const PinHandler BT_RST = createPinHandler(6, 1);
const PinHandler BT_REST = createPinHandler(6, 2);
const PinHandler BT_EN = createPinHandler(6, 3);
const PinHandler BT_BRTS = createPinHandler(6, 4);
const PinHandler BT_BCTS = createPinHandler(2, 0);
const int BT_UART = UART1;

#endif //_PIN_INTERFACE_H_