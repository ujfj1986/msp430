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
    writeStrThroughBluetoothDevice("AUTH", 4);
}

static void bluetoothDisconnectProcess(void* context) {
    // TODO: dule with bluetooth device disconnect event.
    log("in bluetoothDisconnectProcess\n");
}
static int findEnd() {
    int i = 0;
    for(i = 0; i < gBTLock.offset; i++) {
        if (gBTLock.packageBuf[i] == '$') return (i + 1);
    }
    return 0;
}
typedef struct BT_CMD_T {
    char* cmd_start;
    int cmd_start_len;
    int cmd_len;
    int (*process)(struct BT_CMD_T* cmd);
} BT_CMD_T;
int processAuthCmd(BT_CMD_T* cmd);
int processAddCmd(BT_CMD_T* cmd);
int processOpenCmd(BT_CMD_T* cmd);
int processConfigCmd(BT_CMD_T* cmd);
int processRemoveCmd(BT_CMD_T* cmd);
static BT_CMD_T gBtCommands = {
    {"AUTH", 4, 30, processAuthCmd},
    {"OPEN$", 5, 5, processOpenCmd},
    {"CONFIG", 6, 32, processConfigCmd},
    {"ADD", 3, 29, processAddCmd},
    {"REMOVE", 6, 25, processRemoveCmd},
    {NULL, 0, 0, NULL},
};
static int parse() {
    //parse command from package buffer and return command length.
    //if return -1, this means the command just receive a part.
    //if return -2, this means the package buffer doesn't contain any command, clear it.
    //Commands:
    //  "AUTH XX:XX:XX:XX:XX:XX YYYYYY$":        XX is mac address; YYYYYY is cipher.
    //  "OPEN$":                                 open this lock.
    //  "CONFIG XX:XX:XX:XX:XX:XX YYYYYYY$":     configuration this bt device's cipher.
    //  "ADD XX:XX:XX:XX:XX:XX YYYYYY$":         add new bt device.
    //  "REMOVE XX:XX:XX:XX:XX:XX$":             remove bt device.
    if (gBTLock.offset < 5) return -1;
    int end = 0;
    BT_CMD_T* p = NULL;
    for (p = gBtCommands; (p != NULL && p->cmd_start != NULL); p++) {
        if (0 == memcmp(gBTLock.packageBuf, p->cmd_start, p->cmd_start_len)) {
            end = findEnd();
            if (!end) goto part_message;
            else if (p->cmd_len != end) {
                goto error_message;
            } else {
                return end;
            }
        }
    }
    /*if (0 == memcmp(gBTLock.packageBuf, "AUTH", 4)) {
        end = findEnd(); 
        if (!end) goto part_message;
        else if (30 !=  end) {
            goto error_message;
        } else {
            return end;
        }
    }
    if (0 == memcmp(gBTLock.packageBuf, "OPEN$", 5)) {
        return 5;
    }
    if (0 == memcmp(gBTLock.packageBuf, "CONFIG", 6)) {
        end = findEnd();
        if (!end) goto part_message;
        else if (32 != end) {
            goto error_message;
        } else {
            return end;
        }
    }
    if (0 == memcmp(gBTLock.packageBuf, "ADD", 3)) {
        end = findEnd();
        if (!end) goto part_message;
        else if ()
    }*/
    end = findEnd();
    if (!end) return -1;
error_message:
    memcpy(gBTLock.packageBuf, gBTLock.packageBuf + end, gBTLock.offset - end);
    gBTLock.offset -= end;
    return -2;
part_message:
    return -1;
}

static int processCommands(int len) {
    //TODO: process commands. parameter is command len.
    //command is in package buffer.
    char mac[MAC_LEN] = "\0";
    char cipher[KEY_LEN] = "\0";
    int i = 0;

    if (gBTLock.status == UNAUTH) {
        if (30 != len) {
            log("receive %s message on unauth status.\n", ((5 == len) ? "Open" : "Config"));
            writeStrThroughBluetoothDevice("AUTH", 4);

            return -1;
        }

        memcpy(mac, gBTLock.packageBuf + 5, MAC_LEN - 1);
        memcpy(cipher, gBTLock.packageBuf + 23, KEY_LEN - 1);
        
        for (i = 0; i < MAX_CIPHER_NUM; i++) {
            if ((0 == memcmp(mac, gBTLock.config->ciphers[i].mac, MAC_LEN)) &&
            (0 == memcpy(cipher, gBTLock.config->ciphers[i].cipher, KEY_LEN))) {
                gBTLock.status = AUTH;
                break;
            }
        }
        return 0;
    }


    return 0;
}
static void bluetoothReadProcess(void* context) {
    log("in bluetoothReadProcess\n");
    int len = readStrThroughBluetoothDevice(gBTLock.packageBuf + gBTLock.offset,
        PACKAGE_BUF_SIZE - gBTLock.offset);
    log("read from bluetooth: %d %s\n", len, gBTLock.packageBuf + gBTLock.offset);
    gBTLock.offset += len;
    int ret = 0;
    while (1) {
        int ret = parse();
        if (0 >= ret) {
            if (-1 == ret) {
                log("this command just receive a part.\n");
                break;
            } else if(-2 == ret) {
                log("The package buffer doesn\'t contain any commands.\n");
                continue;
            }
            return ;
        }
        processCommands(ret);
    }
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