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

//configure bluetooth device PID.
int configBluetoothPID(char* mac);

//configure bluetooth device broadcast period
typedef enum BluetoothDeviceBroadcastPeriod {
    
}
int configBluetoothBroadcastPeriod(int ms);

// wake up bluetooth device when it sleeps.
int wakeBluetooth();

// bluetooth device sleeps when it wakes.
int sleepBluetooth();

//scan and connect to other bluetooth devices.

//write data through bluetooth.
int writeStrThroughBluetooth(char* str, int len);

//register bluetooth data process method.
/*typedef int (*bluetoothDataProcess)(char* str, int len);
int registerBluetoothDataProcess(bluetoothDataProcess process);*/

#endif //_BLUETOOTH_H_