/*
* bluetoothlock.c - implement the bluetooth lock module.
*
* Author: Shan Jiejing
* Date: 2017-02-01
*/

#include "bluetoothlock.h"
#include "pin.h"
#include "bluetoothdevice.h"
#include "event.h"
#include "log.h"
#include "uart.h"
#include "config.h"
#include "store.h"

#include <stdio.h>

static void bluetoothConnectProcess(void* context) {
    // TODO: dule with bluetooth device connect event.
    log("in bluetoothConnectProcess\n");
    writeStrThroughBluetoothDevice("send", 4);
}

static void bluetoothDisconnectProcess(void* context) {
    // TODO: dule with bluetooth device disconnect event.
    log("in bluetoothDisconnectProcess\n");
}

static void bluetoothReadProcess(void* context) {
    // TODO: dule with bluetooth device read event.
    log("in bluetoothReadProcess\n");
    char buf[100] = "\0";
    int len = readStrThroughBluetoothDevice(buf, 100);
    log("read from bluetooth: %d %s\n", len, buf);
}

static int resetPinProcess(PinHandler pin, void* context) {
    // TODO: dule with reset pin event.
    return 0;
}

int initBluetoothLock() {
//    loadConfig();

    //config reset pin.

    //config bluetooth device.
    initBluetoothDevice(createPinHandler(6,0), //vcc
        createPinHandler(6, 6), //gnd
        createPinHandler(6, 1), //rst
        createPinHandler(6, 2), //rest
        createPinHandler(6, 3), //en
        createPinHandler(6, 4), //brts
        createPinHandler(2, 0), //bcts
        UART1);
    powerOnBluetoothDevice();
    wakeBluetoothDevice();

    //register bluetooth connect/disconnect/read event.
    registerEventProcess(BLUETOOTH_CONNECT, bluetoothConnectProcess, NULL);
    registerEventProcess(BLUETOOTH_DISCONNECT, bluetoothDisconnectProcess, NULL);
    registerEventProcess(BLUETOOTH_READ, bluetoothReadProcess, NULL);
    
    return 0;
}