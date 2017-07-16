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

extern const PinHandler UART0_TX ;
extern const PinHandler UART0_RX ;
extern const PinHandler UART1_TX ;
extern const PinHandler UART1_RX ;

extern const PinHandler I2C_SCL ;
extern const PinHandler I2C_SDA ;

extern const PinHandler KEY_PAD_VCC ;
extern const PinHandler KEY_PAD_GND ;
extern const PinHandler KEY_PAD_IRQ ;
extern const PinHandler KEY_PAD_EN ;
extern const PinHandler KEY_PAD_BG ;
extern const PinHandler KEY_PAD_LED ;

// bluetooth
extern const PinHandler BT_VCC ;
extern const PinHandler BT_GND ;
extern const PinHandler BT_RST ;
extern const PinHandler BT_REST ;
extern const PinHandler BT_EN ;
extern const PinHandler BT_BRTS ;
extern const PinHandler BT_BCTS ;
extern const int BT_UART ;

#endif //_PIN_INTERFACE_H_