/*
* bluetooth.c - implement the bluetooth module.
*
* Author: Shan Jiejing
* Date: 2017-1-9
*/

#include "bluetooth.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum BluetoothDeviceStatus {
    UNINIT = 0,
    INIT = 1,
    STARTED = 2,
    CONNECTED = 3,
    SLEEP = 4,
} BluetoothDeviceStatus;

#define NAMELEN 16
#define PWDLEN 6
#define MACLEN 6
#define DEFAULT_BAUDRATE 9600
#define DEFAULT_PWD "000000"
#define BLUETOOTH_UART UART1
#define BLUETOOTH_BRTS_PIN_M 
#define BLUETOOTH_BRTS_PIN_S
#define BLUETOOTH_BCTS_PIN_M
#define BLUETOOTH_BCTS_PIN_S
#define BLUETOOTH_EN_M
#define BLUETOOTH_EN_S
#define BLUETOOTH_RST_M
#define BLUETOOTH_RST_S
#define BLUETOOTH_REST_M
#define BLUETOOTH_REST_S
#define BLUETOOTH_VCC_M
#define BLUETOOTH_VCC_S
#define BLUETOOTH_GND_M
#define BLUETOOTH_GND_S

#define SET_PIN(M, S) \
    P#M#OUT |= (1 << S)

#define CLR_PIN(M, S) \
    P#M#OUT &= ~(1 << S)

PinHandler createPinHandler(char m, char s) {
    return (char)((m & 0x0F) << 4 | (s & 0x0F));
}

#define getM(h) (char)((h & 0xF0) >> 4)

#define getS(h) (char)(h & 0x0F)

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
    BluetoothDeviceConnectedEventProcess proc;
    void* context;
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
    return 0;
}

//power on bluetooth device
int powerOnBluetoothDevice() {
    if (INIT != device.status) return  -1;

    char m = getM(device.VCC_PIN);
    char s = getS(device.VCC_PIN);

    SET_PIN(m, s); // vcc on
    m = getM(device.GND_PIN);
    s = getS(device.GND_PIN);
    CLR_PIN(m, s); // gnd off

    // rest off
    //rst off
    // en off

    // TODO: register bcts process
}

//power off bluetooth device
int powerOffBluetoothDevice();

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
typedef enum BluetoothDeviceBroadcastPeriod {
    
} BluetoothDeviceBroadcastPeriod;
int configBluetoothDeviceBroadcastPeriod(int ms);

// wake up bluetooth device when it sleeps.
int wakeBluetoothDevice();

// bluetooth device sleeps when it wakes.
int sleepBluetoothDevice();

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetoothDevice(char* str, int len);

//register bluetooth connected event process method.
typedef int (*BluetoothDeviceConnectedEventProcess)(void* context);
int registerBluetoothDeviceConnectedEventProcess(BluetoothDeviceConnectedEventProcess process,
    void* context);
int unregisterBluetoothDeviceConnectedEventProcess();

// read data through bluetooth.
int readStrThroughBluetoothDevice(char* str, int len);
