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

typedef enum BTLockStatus {
    UNAUTH = 0,
    AUTH = 1,
} BTLockStatus;

#define PACKAGE_BUF_SIZE 64
typedef struct BTLock {
    BTLockStatus status;
    char packageBuf[PACKAGE_BUF_SIZE];
    unsigned char offset;
    LockConfig* config;
} BTLock;

static BTLock gBTLock;
static void bluetoothConnectProcess(void* context) {
    // TODO: dule with bluetooth device connect event.
    log("in bluetoothConnectProcess\n");
    writeStrThroughBluetoothDevice("send", 4);
}

static void bluetoothDisconnectProcess(void* context) {
    // TODO: dule with bluetooth device disconnect event.
    log("in bluetoothDisconnectProcess\n");
}

static int parse() {
    //TODO: parse command from package buffer and return command length.
    //if return -1, this means the command just receive a part.
    //if return -2, this means the package buffer doesn't contain any command, clear it.
    //Commands:
    //  "AUTH XX:XX:XX:XX:XX:XX YYYYYY$":        XX is mac address; YYYYYY is cipher.
    //  "OPEN$":                                 open this lock.
    //  "CONFIG XX:XX:XX:XX:XX:XX YYYYYYY$":     configuration this bt device's cipher.
    return 0;
}

static int processCommands(int len) {
    //TODO: process commands. parameter is command len.
    //command is in package buffer.
    return 0;
}
static void bluetoothReadProcess(void* context) {
    log("in bluetoothReadProcess\n");
    int len = readStrThroughBluetoothDevice(gBTLock.packageBuf + gBTLock.offset,
        PACKAGE_BUF_SIZE - gBTLock.offset);
    log("read from bluetooth: %d %s\n", len, gBTLock.packageBuf + gBTLock.offset);
    gBTLock.offset += len;
    int ret = parse();
    if (0 >= ret) {
        if (-1 == ret) {
            log("this command just receive a part.\n");
        } else if(-2 == ret) {
            log("The package buffer doesn\'t contain any commands.\n");
            memset(gBTLock.packageBuf, 0, gBTLock.offset);
            gBTLock.offset = 0;
        }
        log("parse error.\n");
        return ;
    }
    processCommands(ret);
}

static int resetPinProcess(PinHandler pin, void* context) {
    // TODO: dule with reset pin event.
    return 0;
}

int initBluetoothLock(Lock* lock) {
    memset(&gBTLock, 0, sizeof(gBTLock));
    gBTLock.config = &(lock->config);
    gBTLock.status = UNAUTH;

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
    
    //TODO: configuration bt name and pin
    return 0;
}