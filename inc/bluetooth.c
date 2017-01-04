/*
* bluetooth.c - implement the bluetooth module.
*
* Author: Shan Jiejing
* Date: 2017-1-9
*/

#include "bluetooth.h"

typedef enum BluetoothDeviceStatus {
    UNINIT = 0,
    STARTED = 1,
    CONNECTED = 2,
    SLEEP = 3,
} BluetoothDeviceStatus;

#define NAMELEN 16
#define PWDLEN 6
#define MACLEN 6
#define DEFAULT_BAUDRATE 9600
typedef struct BluetoothDevice {
    BluetoothDeviceStatus status;
    char name[NAMELEN];
    int connectPeriod;
    char pwd[PWDLEN];
    char mac[MACLEN];

}