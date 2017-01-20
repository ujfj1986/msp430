/*
* pin.c - implement the pin control module.
*
* Author: Shan Jiejing
* Date: 2017-01-20
*/
#include <string.h>

#include "pin.h"

PinHandler createPinHandler(char pinsId, char pinId) {
    return (PinHandler)((pinsId & 0x0f) << 4 | (pinId & 0x0f));
}

char getPinsId(PinHandler pin) {
    return (char) ((pin & 0xf0) >> 4);
}

char getPinId(PinHandler pin) {
    return (char) (pin & 0x0f);
}

typedef struct Pin {
    PinStatus status;
    PinUpProc proc;
    void* context;
} Pin;

#define PINLINES 8
#define PINSCOUNT 6
#define REG_IN(x) P##x##IN
#define REG_OUT(x) P##x##OUT
#define REG_SEL(x) P##x##SEL
#define REG_DIR(x) P##x##DIR
#define REG_IFG(x) P##x##IFG
#define REG_IE(x) P##x##IE
#define REG_IES(x) P##x##IES
#define SET_BIT(reg, bit) reg |= (1 << bit)
#define CLR_BIT(reg, bit) reg &= ~(1 << bit)

typedef struct Pins {
    PinStatus pins_status;
    Pin pin[PINLINES];
} Pins;

static Pins pins[PINSCOUNT];

int initPins() {
    char id = 0;
    memset(&pins, 0, sizeof(pins));

    for (id = 0; id < PINSCOUNT; id ++) {
        configPinsStatus(id, OUT);
        setPinsValue(id, 0xFF);
    }

    return 0;
}

char getPinValue(PinHandler);

char getPinsValue(char pinsId);

int setPinValue(PinHandler, char value);

int setPinsValue(char pinsId, char value);

int configPinsStatus(char pinsId, PinStatus status) {
    if (0 >= pinsId || PINSCOUNT < pinsId) return -1;

    char id  = 0;
    P
    pins[pinsId - 1].pins_status = status;
    for (id = 0; id < PINLINES; id ++) {
        pins[pinsId - 1].pin[id].status = status;
    }

    switch (status) {
        case IN:
            REG_SEL(pinsId) = 0x00;
            REG_DIR(pinsId) = 0x00;
            break;
        case OUT:
            REG_SEL(pinsId) = 0x00;
            REG_DIR(pinsId) = 0xFF;
            break;
        case SEL:
            REG_SEL(pinsId) = 0xFF;
            break;
    }

    return 0;
}

int configPinStatus(PinHandler pin, PinStatus status) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 >= pinsId || PINSCOUNT < pinsId) ||
    (0 > pinId || PINLINES <= pinId)) return -1;

    pins[pinsId - 1].pin[pinId].status = status;

    switch (status) {
        case IN:
            CLR_BIT(REG_SEL(pinsId), pinId);
            CLR_BIT(REG_DIR(pinsId), pinId);
            break;
        case OUT:
            CLR_BIT(REG_SEL(pinsId), pinId);
            SET_BIT(REG_DIR(pinsId), pinId);
            break;
        case SEL:
            SET_BIT(REG_SEL(pinsId), pinId);
            break;
    }

    return 0;
}

typedef int (*PinUpProc)(PinHandler, void*);
int registerPinUpProc(PinHandler, PinUpProc, void*);

int unregisterPinUpProc(PinHandler);

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void);

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void);