/*
* bluetooth.c - implement the bluetooth module.
*
* Author: Shan Jiejing
* Date: 2017-1-9
*/

#include "bluetoothdevice.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "buffer.h"
#include "uart.h"
#include "timer.h"
#include "log.h"
#include "pin_interface.h"

typedef enum BluetoothDeviceStatus {
    UNINIT = 0,
    INIT = 1,
    POWER_ON = 2,
    STARTED = 3,
    CONNECTED = 4,
    CONFIGING = 5,
} BluetoothDeviceStatus;

#define NAMELEN 16
#define PWDLEN 6
#define MACLEN 6
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_PWD "000000"
#define BLUETOOTH_UART UART1
#define CMDSTART "TTM:"
#define TTMOK "TTM:OK\r\n"
#define TTMERROR "TTM:ERR\r\n"
#define TTMDISCONNECT "TTM:DISCONNECT\r\n"
#define TTMTIMEOUT "TTM:DISCONNECT FOR TIMEOUT\r\n"

typedef struct BluetoothDevice {
    BluetoothDeviceStatus status;
    char name[NAMELEN];
    int connectPeriod;
    char pwd[PWDLEN];
    char mac[MACLEN];
    /*PinHandler VCC_PIN;
    PinHandler GND_PIN;
    PinHandler RST_PIN;
    PinHandler REST_PIN;
    PinHandler EN_PIN;
    PinHandler BRTS_PIN;
    PinHandler BCTS_PIN;
    int uartHandler; */
    // BluetoothDeviceConnectedEventProcess proc;
    // void* context;
    struct Buffer rBuffer;
} BluetoothDevice;

static BluetoothDevice device;

/*int initBluetoothDevice(PinHandler vcc,
    PinHandler gnd,
    PinHandler rst,
    PinHandler rest,
    PinHandler en,
    PinHandler brts,
    PinHandler bcts,
    int uartId) {*/
int initBluetoothDevice() {
    if (device.status != UNINIT) return 0;
    
    memset(&device, 0, sizeof(device));

    /*device.VCC_PIN = vcc;
    device.GND_PIN = gnd;
    device.RST_PIN = rst;
    device.REST_PIN = rest;
    device.EN_PIN = en;
    device.BRTS_PIN = brts;
    device.BCTS_PIN = bcts;
    device.uartHandler = uartId;*/
    device.status = INIT;
    configPinStatus(BT_VCC, PIN_OUT);
    setPinValue(BT_VCC, 0);
    configPinStatus(BT_GND, PIN_OUT);
    setPinValue(BT_GND, 0);
    configPinStatus(BT_RST, PIN_OUT);
    setPinValue(BT_RST, 0);
    configPinStatus(BT_REST, PIN_OUT);
    setPinValue(BT_REST, 0);
    configPinStatus(BT_EN, PIN_OUT);
    setPinValue(BT_EN, 0);
    configPinStatus(BT_BRTS, PIN_OUT);
    setPinValue(BT_BRTS, 0);
    configPinStatus(BT_BCTS, PIN_IN);
    openUart(BT_UART);
    initBuffer(&(device.rBuffer));
    return 0;
}

static void uartReadProcess(void * context) {
    unsigned char buf[100] = "\0";
    int i = 0;
    log("in uartReadProcess\n");
    int len = 0;

    len = readStrFrom(BT_UART, buf, 100);
    log("read str %d, %x, %x, %s.\n", len, buf[0], buf[1], buf);

    if (0 == strncmp((char*)buf, CMDSTART, strlen(CMDSTART))) {
        if (0 == strncmp((char*)buf, TTMOK, strlen(TTMOK))) {
            if (STARTED == device.status) {
                //connect successfully
                device.status = CONNECTED;
                //raise bluetooth connected event
                raiseEvent(BLUETOOTH_CONNECT);
                return ;
            } else if (CONFIGING == device.status) {
                // config success.
            }

        }
        if (0 == strncmp((char*)buf, TTMDISCONNECT, strlen(TTMDISCONNECT)) ||
            0 == strncmp((char*)buf, TTMTIMEOUT, strlen(TTMTIMEOUT))) {
            // raise bluetooth disconnected event
            raiseEvent(BLUETOOTH_DISCONNECT);
            device.status = STARTED;
            return ;
        }
    } else {
        for (i = 0; i < len; i++) writeBuffer(&(device.rBuffer), buf[i]);
        raiseEvent(BLUETOOTH_READ);
    }
}
static int bctsHandler(PinHandler bcts, void* context) {
    //unsigned char buf[100] = "\0";
    //int i = 0;
    log("in bctsHandler\n");

    // don't do anything
    /*int len = readStrFrom(device.uartHandler, buf, 100);
    log("read str %d, %s.\n", len, buf);
    if (0 >= len) {
        //sleep 500 ms to wait
        delay_ms(500);
        len = readStrFrom(device.uartHandler, buf, 100);
        log("read2 str %d, %s.\n", len, buf);
        if (0 >= len) {
            return -1;
        }
    }

    if (0 == strncmp((char*)buf, CMDSTART, strlen(CMDSTART))) {
        if (0 == strncmp((char*)buf, TTMOK, strlen(TTMOK))) {
            //connect successfully
            device.status = CONNECTED;
            //raise bluetooth connected event
            // if (NULL == device.proc) {
            //     device.proc(device.context);
            // }
            raiseEvent(BLUETOOTH_CONNECT);
            return 0;
        }
        if (0 == strncmp((char*)buf, TTMDISCONNECT, strlen(TTMDISCONNECT)) ||
            0 == strncmp((char*)buf, TTMTIMEOUT, strlen(TTMTIMEOUT))) {
            // raise bluetooth disconnected event
            raiseEvent(BLUETOOTH_DISCONNECT);
            device.status = STARTED;
            return 0;
        }
    } else {
        for (i = 0; i < len; i++) writeBuffer(&(device.rBuffer), buf[i]);
        raiseEvent(BLUETOOTH_READ);
    }*/
    return 0;
}
//power on bluetooth device
int powerOnBluetoothDevice() {
    if (INIT != device.status) return  -1;
    log("in powerOnBluetoothDevice\n");

    setPinValue(BT_VCC, 1); // vcc on

    setPinValue(BT_GND, 0); // gnd off

    setPinValue(BT_REST, 1);// rest up
    setPinValue(BT_RST, 1);//rst off
    setPinValue(BT_EN, 1);// en off
    setPinValue(BT_BRTS, 1);

    //  register bcts process
    //registerPinProc(device.BCTS_PIN, 1, bctsHandler, (void*)&device);
    // register uart read process
    registerEventProcess((BT_UART == UART0 ? UART0READ : UART1READ),
        uartReadProcess, NULL);
    device.status = POWER_ON;
    return 0;
}

//power off bluetooth device
int powerOffBluetoothDevice() {
    if (INIT >= device.status) return 0;

    setPinValue(BT_VCC, 0);
    setPinValue(BT_REST, 0);
    setPinValue(BT_RST, 0);
    setPinValue(BT_EN, 0);
    setPinValue(BT_BRTS, 0);

    // unregister bcts process
    //unregisterPinProc(device.BCTS_PIN);
    unregisterEventProcess((BT_UART == UART0 ? UART0READ : UART1READ));
    device.status = INIT;
    return 0;
}

//configure bluetooth device name.
int configBluetoothDeviceName(char* name, int len);

//configure bluetooth device connect period.
int configBluetoothDeviceConnectPeriod(int ms);

//configure bluetooth device password.
int configBluetoothDevicePassword(char* pwd, int len);

//configure bluetooth device MAC address.
int configBluetoothDeviceMAC(char* mac, int len);

//configure bluetooth device PID.
int configBluetoothDevicePID(char* mac);

//configure bluetooth device broadcast period
int configBluetoothDeviceBroadcastPeriod(BluetoothDeviceBroadcastPeriod period);

// wake up bluetooth device when it sleeps.
int wakeBluetoothDevice() {
    if (POWER_ON != device.status) return -1;

    setPinValue(BT_EN, 0);
    device.status = STARTED;
    return 0;
}

// bluetooth device sleeps when it wakes.
int sleepBluetoothDevice() {
    if (POWER_ON >= device.status) return -1;

    setPinValue(BT_EN, 1);
    device.status = POWER_ON;
    return 0;
}

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetoothDevice(char* str, int len) {
    if (CONNECTED != device.status || NULL == str || 0 >= len) return -1;
    int t = 0;

    //down the BRTS pin.
    while(1 != getPinValue(BT_BCTS));
    setPinValue(BT_BRTS, 0);
    do {
        t += writeStrTo(BT_UART, (unsigned char*)str + t, len - t);
    } while(t < len);

    setPinValue(BT_BRTS, 1);
    return t;
}

//register bluetooth connected event process method.
// int registerBluetoothDeviceConnectedEventProcess(BluetoothDeviceConnectedEventProcess process,
//     void* context) {
//         if (NULL == process || UNINIT == device.status || NULL != device.proc) return -1;
        
//         device.proc = process;
//         device.context = context;
//         return 0;
//     }
// int unregisterBluetoothDeviceConnectedEventProcess() {
//     device.proc = NULL;
//     device.context = NULL;
//     return 0;
// }

// read data through bluetooth.
int readStrThroughBluetoothDevice(char* str, int len) {
    if (CONNECTED != device.status || NULL == str || 0 >= len) return -1;
    return readBuffer(&(device.rBuffer), (unsigned char*)str, len);
}
