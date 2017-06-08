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

#define KEY_LEN (6+1)
#define MAC_LEN 18
#define MAX_CIPHER_NUM 5
#define BTNAME_LEN (16+1)

typedef struct Cipher {
    unsigned char mac[MAC_LEN];
    unsigned char pwd[KEY_LEN];
} Cipher;

typedef struct LockConfig {
    char isFirstBoot;
    unsigned char key[KEY_LEN];
    char btName[BTNAME_LEN];
    unsigned char bluetoothPin[KEY_LEN];
    Cipher ciphers[MAX_CIPHER_NUM];
} LockConfig;

int loadConfig(LockConfig* config);
int updateConfig(LockConfig* config);

#endif //_CONFIG_H_