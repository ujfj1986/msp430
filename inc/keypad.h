#ifndef _KEYPAD_H_
#define _KEYPAD_H_
void Init_Keypad(void);
void Check_Key(void);
void delay();
void Key_Event(void);
unsigned char key_val;          //存放键值
unsigned char key_Flag;         //按键是否已放开：1--是，0--否
#endif //_KEYPAD_H_