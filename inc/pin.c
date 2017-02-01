/*
* pin.c - implement the pin control module.
*
* Author: Shan Jiejing
* Date: 2017-01-20
*/
#include <string.h>
#include <msp430x14x.h>

#include "pin.h"
#include "event.h"

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
    char irqType;
    PinProc proc;
    void* context;
} Pin;

#define PINLINES 8
#define PINSCOUNT 6

unsigned char const volatile* REG_IN(char x) {
    switch (x) {
        case 1:
            return &P1IN;
        case 2:
            return &P2IN;
        case 3:
            return &P3IN;
        case 4:
            return &P4IN;
        case 5:
            return &P5IN;
        case 6:
            return &P6IN;
    }
    return NULL;
}
unsigned char volatile* REG_OUT(char x) {
    switch (x) {
        case 1:
            return &P1OUT;
        case 2:
            return &P2OUT;
        case 3:
            return &P3OUT;
        case 4:
            return &P4OUT;
        case 5:
            return &P5OUT;
        case 6:
            return &P6OUT;
    }
    return NULL;
}
unsigned char volatile* REG_SEL(char x) {
    switch (x) {
        case 1:
            return &P1SEL;
        case 2:
            return &P2SEL;
        case 3:
            return &P3SEL;
        case 4:
            return &P4SEL;
        case 5:
            return &P5SEL;
        case 6:
            return &P6SEL;
    }
    return NULL;
}
unsigned char volatile* REG_DIR(char x) {
    switch (x) {
        case 1:
            return &P1DIR;
        case 2:
            return &P2DIR;
        case 3:
            return &P3DIR;
        case 4:
            return &P4DIR;
        case 5:
            return &P5DIR;
        case 6:
            return &P6DIR;
    }
    return NULL;
}
unsigned char volatile* REG_IFG(char x) {
    switch (x) {
        case 1:
            return &P1IFG;
        case 2:
            return &P2IFG;
    }
    return NULL;
}
unsigned char volatile* REG_IE(char x) {
    switch (x) {
        case 1:
            return &P1IE;
        case 2:
            return &P2IE;
    }
    return NULL;
}
unsigned char volatile* REG_IES(char x) {
    switch (x) {
        case 1:
            return &P1IES;
        case 2:
            return &P2IES;
    }
    return NULL;
}
#define SET_BIT(reg, bit) reg |= (1 << bit)
#define CLR_BIT(reg, bit) reg &= ~(1 << bit)
#define GET_BIT(reg, bit) (char)(((reg) & (1 << bit)) >> bit)

typedef unsigned short PinsStatus;
#define PINS_STATUS_ALL_IN 0x0000
#define PINS_STATUS_ALL_OUT 0x5555
#define PINS_STATUS_ALL_SEL 0xAAAA
static PinStatus getPinStatus(PinsStatus pins, char id) {
    unsigned short tmp = (unsigned short)((pins & (0x0003 << (2 * id))) >> (2 * id));
    return (PinStatus)tmp;
}
static void setPinStatus(PinsStatus* pins, char id, PinStatus pin) {
    *pins &= ~(0x0003 << (2 * id)); //clear that bits.
    unsigned short tmp = pin;
    *pins |= (tmp << (2 * id));
}
typedef struct Pins {
    PinsStatus pins_status;
    char hasIrq;
    char irqValue;
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

char getPinValue(PinHandler pin) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 >= pinsId || PINSCOUNT < pinsId) ||
        (0 > pinId || PINLINES <= pinId)) return -1;
    
    PinStatus status = getPinStatus(pins[pinsId - 1].pins_status, pinId);
    if (PIN_IN != status) return -1;

    unsigned char const volatile* reg = REG_IN(pinsId);
    return GET_BIT(*reg, pinId);
}

char getPinsValue(char pinsId) {
    if (0 >= pinsId || PINSCOUNT < pinsId) return -1;

    if (PINS_STATUS_ALL_IN != pins[pinsId - 1].pins_status) return -1;

    return *(REG_IN(pinsId)); 
}

int setPinValue(PinHandler pin, char value) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 >= pinsId || PINSCOUNT < pinsId) ||
        (0 > pinId || PINLINES <= pinId)) return -1;
    
    PinStatus status = getPinStatus(pins[pinsId - 1].pins_status, pinId);
    if (PIN_OUT != status) return -1;
    unsigned char volatile* reg = REG_OUT(pinsId);
    (value == 0) ? (CLR_BIT(*reg, pinId)) :
            (SET_BIT(*reg, pinId));
    return 0;
}

int setPinsValue(char pinsId, char value) {
    if (0 >= pinsId || PINSCOUNT < pinsId) return -1;

    if (PINS_STATUS_ALL_OUT != pins[pinsId - 1].pins_status) return -1;

    *(REG_OUT(pinsId)) = value;
    return 0;
}

int configPinsStatus(char pinsId, PinStatus status) {
    if (0 >= pinsId || PINSCOUNT < pinsId) return -1;
    
    pins[pinsId - 1].pins_status = (status == PIN_IN) ?
        (PINS_STATUS_ALL_IN) :
        ((status == PIN_OUT) ?
        (PINS_STATUS_ALL_OUT) :
        (PINS_STATUS_ALL_SEL));
    /*for (id = 0; id < PINLINES; id ++) {
        pins[pinsId - 1].pin[id].status = status;
    }*/

    unsigned char volatile* reg_sel = REG_SEL(pinsId);
    unsigned char volatile* reg_dir = REG_DIR(pinsId);
    switch (status) {
        case PIN_IN:
            *(reg_sel) = 0x00;
            *(reg_dir) = 0x00;
            break;
        case PIN_OUT:
            *(reg_sel) = 0x00;
            *(reg_dir) = 0xFF;
            break;
        case PIN_SEL:
            *(reg_sel) = 0xFF;
            break;
    }

    return 0;
}

int configPinStatus(PinHandler pin, PinStatus status) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 >= pinsId || PINSCOUNT < pinsId) ||
    (0 > pinId || PINLINES <= pinId)) return -1;

    //pins[pinsId - 1].pin[pinId].status = status;
    setPinStatus(&(pins[pinsId - 1].pins_status), pinId, status);

    unsigned char volatile* reg_sel = REG_SEL(pinsId);
    unsigned char volatile* reg_dir = REG_DIR(pinsId);
    switch (status) {
        case PIN_IN:
            CLR_BIT(*reg_sel, pinId);
            CLR_BIT(*reg_dir, pinId);
            break;
        case PIN_OUT:
            CLR_BIT(*reg_sel, pinId);
            SET_BIT(*reg_dir, pinId);
            break;
        case PIN_SEL:
            SET_BIT(*reg_sel, pinId);
            break;
    }

    return 0;
}

static void handleIrq(char pinsId) {
    char i = 0;
    char t = 0;
    char ifg = pins[pinsId - 1].irqValue;
    Pins *p = &(pins[pinsId - 1]);

    for (i = 0; i < PINLINES; i++) {
        t = ((ifg & (1 << i)) >> i);
        if (0 != t) {
            p->pin[i].proc(createPinHandler(pinsId, i), p->pin[i].context);
        } 
    }
}
static void pinIrqHandler(void* context) {
    (void *)context;

    if (!pins[0].hasIrq || !pins[1].hasIrq) return;

    if (pins[0].hasIrq) {
        handleIrq(1);
        pins[0].hasIrq = 0;
        pins[0].irqValue = 0;
    }

    if (pins[1].hasIrq) {
        handleIrq(2);
        pins[1].hasIrq = 0;
        pins[1].irqValue = 0;
    }

}

int registerPinProc(PinHandler pin, char type, PinProc proc, void* context) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 != pinsId && 1 != pinsId) ||
        (0 > pinId || PINLINES <= pinId) ||
        (NULL == proc)) return -1;

    PinStatus status = getPinStatus(pins[pinsId - 1].pins_status, pinId);
    if (PIN_IN != status) return -1;
    Pin* p = &(pins[pinsId - 1].pin[pinId]);
    if (NULL != p->proc) return -1;

    //pins[pinsId - 1].hasIrq = 1;
    p->irqType = type;
    p->proc = proc;
    p->context = context;
    unsigned char volatile* reg_ies = REG_IES(pinsId);
    unsigned char volatile* reg_ifg = REG_IFG(pinsId);
    unsigned char volatile* reg_ie = REG_IE(pinsId);
    (0 == type) ?
        (CLR_BIT(*reg_ies, pinId)) :
        (SET_BIT(*reg_ies, pinId));
    CLR_BIT(*reg_ifg, pinId);
    //register pin irq event process
    registerEventProcess(PIN_IRQ, pinIrqHandler, NULL);
    SET_BIT(*reg_ie, pinId);

    return 0;
}

int unregisterPinProc(PinHandler pin) {
    char pinsId = getPinsId(pin);
    char pinId = getPinId(pin);

    if ((0 != pinsId && 1 != pinsId) ||
        (0 > pinId || PINLINES <= pinId)) return -1;

    PinStatus status = getPinStatus(pins[pinsId - 1].pins_status, pinId);
    if (PIN_IN != status) return -1;
    Pin* p = &(pins[pinsId - 1].pin[pinId]);
    if (NULL == p->proc) return 0;

    //pins[pinsId - 1].hasIrq = 1;
    p->irqType = 0;
    p->proc = NULL;
    p->context = NULL;
    unsigned char volatile* reg_ies = REG_IES(pinsId);
    unsigned char volatile* reg_ifg = REG_IFG(pinsId);
    unsigned char volatile* reg_ie = REG_IE(pinsId);
    CLR_BIT(*reg_ies, pinId);
    CLR_BIT(*reg_ifg, pinId);
    //register pin irq event process
    //registerEventProcess(PIN_IRQ, pinIrqHandler, NULL);
    CLR_BIT(*reg_ie, pinId);

    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    char tmpIfg = P1IFG;
    char tmpIn = P1IN;

    pins[0].hasIrq = 1;
    pins[0].irqValue = tmpIfg;
    P1IFG = 0;
    raiseEvent(PIN_IRQ);
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
    char tmpIfg = P2IFG;
    char tmpIn = P2IN;

    pins[1].hasIrq = 1;
    pins[1].irqValue = tmpIfg;
    P2IFG = 0;
    raiseEvent(PIN_IRQ);
}