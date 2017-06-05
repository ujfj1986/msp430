/*
* config.h - define the Config structure.
*
* Config structure is used to store the global lock config.
*
* Author: Shan Jiejing.
* Date: 2017.03.26
*/
#ifndef _CONFIG_H_

#define _CONFIG_H_

#define KEY_LEN 6
#define MAC_LEN 6
#define MAX_MAC_NUM 5

typedef struct LockConfig {
    char isFirstBoot;
    unsigned char key[KEY_LEN];
    unsigned char bluetoothPin[KEY_LEN];
    unsigned char macs[MAX_MAC_NUM][MAC_LEN];
} LockConfig;

int loadConfig(LockConfig* config);
int updateConfig(LockConfig* config);

#endif //_CONFIG_H_