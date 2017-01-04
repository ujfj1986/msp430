/*
* bluetooth.h - define the bluetooth module interface.
*
* The bluetooth module is used to control and configure
* the ble bluetooth device.
*
* Author: Shan Jiejing
* Date: 2017-1-4
*/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

// initialize bluetooth device.
int initBluetooth();

//configure bluetooth device name.
int configBluetoothName(char* name, int len);

//configure bluetooth device connect period.
int configBluetoothConnectPeriod(int ms);

//configure bluetooth device password.
int configBluetoothPassword(char* pwd, int len);

//configure bluetooth device MAC address.
int configBluetoothMAC(char* mac, int len);

// wake up bluetooth device when it sleeps.
int wakeBluetooth();

// bluetooth device sleeps when it wakes.
int sleepBluetooth();

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetooth(char* str, int len);

//register bluetooth data process method.
typedef int (*bluetoothDataProcess)(char* str, int len);
int registerBluetoothDataProcess(bluetoothDataProcess process);

#endif //_BLUETOOTH_H_