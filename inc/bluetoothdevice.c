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

typedef enum BluetoothDeviceStatus {
    UNINIT = 0,
    INIT = 1,
    POWER_ON = 2,
    STARTED = 3,
    CONNECTED = 4,
} BluetoothDeviceStatus;

#define NAMELEN 16
#define PWDLEN 6
#define MACLEN 6
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_PWD "000000"
#define BLUETOOTH_UART UART1
#define CMDSTART "TTM:"
#define TTMOK "TTM:OK\r\n"
#define TTMDISCONNECT "TTM:DISCONNECT\r\n"
#define TTMTIMEOUT "TTM:DISCONNECT FOR TIMEOUT\r\n"

typedef struct BluetoothDevice {
    BluetoothDeviceStatus status;
    char name[NAMELEN];
    int connectPeriod;
    char pwd[PWDLEN];
    char mac[MACLEN];
    PinHandler VCC_PIN;
    PinHandler GND_PIN;
    PinHandler RST_PIN;
    PinHandler REST_PIN;
    PinHandler EN_PIN;
    PinHandler BRTS_PIN;
    PinHandler BCTS_PIN;
    int uartHandler; 
    // BluetoothDeviceConnectedEventProcess proc;
    // void* context;
    struct Buffer rBuffer;
} BluetoothDevice;

static BluetoothDevice device;

int initBluetoothDevice(PinHandler vcc,
    PinHandler gnd,
    PinHandler rst,
    PinHandler rest,
    PinHandler en,
    PinHandler brts,
    PinHandler bcts,
    int uartId) {
    if (device.status != UNINIT) return 0;
    
    memset(&device, 0, sizeof(device));

    device.VCC_PIN = vcc;
    device.GND_PIN = gnd;
    device.RST_PIN = rst;
    device.REST_PIN = rest;
    device.EN_PIN = en;
    device.BRTS_PIN = brts;
    device.BCTS_PIN = bcts;
    device.uartHandler = uartId;
    device.status = INIT;
    configPinStatus(device.VCC_PIN, PIN_OUT);
    setPinValue(device.VCC_PIN, 0);
    configPinStatus(device.GND_PIN, PIN_OUT);
    setPinValue(device.GND_PIN, 0);
    configPinStatus(device.RST_PIN, PIN_OUT);
    setPinValue(device.RST_PIN, 0);
    configPinStatus(device.REST_PIN, PIN_OUT);
    setPinValue(device.REST_PIN, 0);
    configPinStatus(device.EN_PIN, PIN_OUT);
    setPinValue(device.EN_PIN, 0);
    configPinStatus(device.BRTS_PIN, PIN_OUT);
    setPinValue(device.BRTS_PIN, 0);
    configPinStatus(device.BCTS_PIN, PIN_IN);
    openUart(device.uartHandler);
    initBuffer(&(device.rBuffer));
    return 0;
}

static int bctsHandler(PinHandler bcts, void* context) {
    unsigned char buf[100] = "\0";
    int i = 0;

    int len = readStrFrom(device.uartHandler, buf, 100);
    if (0 >= len) {
        //sleep 500 ms to wait
        delay_ms(500);
        len = readStrFrom(device.uartHandler, buf, 100);
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
    }
    return 0;
}
//power on bluetooth device
int powerOnBluetoothDevice() {
    if (INIT != device.status) return  -1;

    setPinValue(device.VCC_PIN, 1); // vcc on

    setPinValue(device.GND_PIN, 0); // gnd off

    setPinValue(device.REST_PIN, 1);// rest up
    setPinValue(device.RST_PIN, 1);//rst off
    setPinValue(device.EN_PIN, 1);// en off
    setPinValue(device.BRTS_PIN, 1);

    //  register bcts process
    registerPinProc(device.BCTS_PIN, 1, bctsHandler, (void*)&device);
    device.status = POWER_ON;
    return 0;
}

//power off bluetooth device
int powerOffBluetoothDevice() {
    if (INIT >= device.status) return 0;

    setPinValue(device.VCC_PIN, 0);
    setPinValue(device.REST_PIN, 0);
    setPinValue(device.RST_PIN, 0);
    setPinValue(device.EN_PIN, 0);
    setPinValue(device.BRTS_PIN, 0);

    // unregister bcts process
    unregisterPinProc(device.BCTS_PIN);
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

    setPinValue(device.EN_PIN, 0);
    device.status = STARTED;
    return 0;
}

// bluetooth device sleeps when it wakes.
int sleepBluetoothDevice() {
    if (POWER_ON >= device.status) return -1;

    setPinValue(device.EN_PIN, 1);
    device.status = POWER_ON;
    return 0;
}

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetoothDevice(char* str, int len) {
    if (CONNECTED != device.status || NULL == str || 0 >= len) return -1;
    int t = 0;

    //down the BRTS pin.
    setPinValue(device.BRTS_PIN, 0);
    do {
        t += writeStrTo(device.uartHandler, (unsigned char*)str + t, len - t);
    } while(t < len);

    setPinValue(device.BRTS_PIN, 1);
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