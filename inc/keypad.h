#ifndef _KEYPAD_H_
#define _KEYPAD_H_
void Init_Keypad(void);
void Check_Key(void);
void delay();
void Key_Event(void);
unsigned char key_val;          //��ż�ֵ
unsigned char key_Flag;         //�����Ƿ��ѷſ���1--�ǣ�0--��
#endif //_KEYPAD_H_