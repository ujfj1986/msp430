/*
* bluetooth.h - define the bluetooth module interface.
*
* The bluetooth module is used to control and configure
* the ble bluetooth device. After initBluetooth, bluetooth
* device is waked, and then it can be configured, and be connected.
* After it connects other bluetooth device, it can gets datas or
* sends datas to another device.
* If it get datas, it will raise BLUETOOTH_READ event.
* If it connects another device successfully, it will raise BLUETOOTH_CONNECT event.
* User can call sleepBluetooth to make the bluetooth device to sleep, and it
* will disconnect with the other bluetooth device.
* If User want to send datas, bluetooth device must be waked.
*
* Author: Shan Jiejing
* Date: 2017-1-4
*/
#ifndef _BLUETOOTHDEVICE_H_
#define _BLUETOOTHDEVICE_H_

#include "pin.h"

// initialize bluetooth device.
/*int initBluetoothDevice(PinHandler vcc,
    PinHandler gnd,
    PinHandler rst,
    PinHandler rest,
    PinHandler en,
    PinHandler brts,
    PinHandler bcts,
    int uartId);*/
int initBluetoothDevice();
//power on bluetooth device
int powerOnBluetoothDevice();

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
    PERIOD_2 = 0,
    PERIOD_5 = 1,
    PERIOD_10 = 2,
    PERIOD_15 = 3,
    PERIOD_20 = 4,
    PERIOD_25 = 5,
    PERIOD_30 = 6,
    PERIOD_40 = 7,
    PERIOD_50 = 8,
} BluetoothDeviceBroadcastPeriod;
int configBluetoothDeviceBroadcastPeriod(BluetoothDeviceBroadcastPeriod period);

// wake up bluetooth device when it sleeps.
int wakeBluetoothDevice();

// bluetooth device sleeps when it wakes.
int sleepBluetoothDevice();

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetoothDevice(char* str, int len);

//register bluetooth connected event process method.
// typedef int (*BluetoothDeviceConnectedEventProcess)(void* context);
// int registerBluetoothDeviceConnectedEventProcess(BluetoothDeviceConnectedEventProcess process,
//     void* context);
// int unregisterBluetoothDeviceConnectedEventProcess();

// read data through bluetooth.
int readStrThroughBluetoothDevice(char* str, int len);
 
#endif //_BLUETOOTHDEVICE_H_