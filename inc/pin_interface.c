/*
* pin_interface.c - implement the pin define.
*
* Author: Shan Jiejing
* Date: 2017-07-17
*/

#include "pin_interface.h"

extern const PinHandler UART0_TX = createPinHandler(3, 4);
extern const PinHandler UART0_RX = createPinHandler(3, 5);
extern const PinHandler UART1_TX = createPinHandler(3, 6);
extern const PinHandler UART1_RX = createPinHandler(3, 7);

extern const PinHandler I2C_SCL = createPinHandler(0, 0);
extern const PinHandler I2C_SDA = createPinHandler(0, 0);

extern const PinHandler KEY_PAD_VCC = createPinHandler(0, 0);
extern const PinHandler KEY_PAD_GND = createPinHandler(0, 0);
extern const PinHandler KEY_PAD_IRQ = createPinHandler(0, 0);
extern const PinHandler KEY_PAD_EN = createPinHandler(0, 0);
extern const PinHandler KEY_PAD_BG = createPinHandler(0, 0);
extern const PinHandler KEY_PAD_LED = createPinHandler(0, 0);

// bluetooth
extern const PinHandler BT_VCC = createPinHandler(6, 0);
extern const PinHandler BT_GND = createPinHandler(6, 6);
extern const PinHandler BT_RST = createPinHandler(6, 1);
extern const PinHandler BT_REST = createPinHandler(6, 2);
extern const PinHandler BT_EN = createPinHandler(6, 3);
extern const PinHandler BT_BRTS = createPinHandler(6, 4);
extern const PinHandler BT_BCTS = createPinHandler(2, 0);
extern const int BT_UART = UART1;

extern const int LOG_UART = UART0;
