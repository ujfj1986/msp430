/*
* bluetoothlock.h - define the interfaces of bluetoothlock.
*
* bluetoothlock is the core module of this lock. It will dule with the
* events of bluetooth device, keypad and other device. If it decides
* the lock should be opened, it opens the lock.
*
* Author: Shan Jiejing
* Date: 2017-02-01
*/

#ifndef _BLUETOOTHLOCK_H_
#define _BLUETOOTHLOCK_H_

int initBluetoothLock();

#endif //_BLUETOOTHLOCK_H_