#ifndef __PWM_H
#define __PWM_H

#include <reg52.h>
#include <math.h>
#include "lcd1602.h"

#define  F_osc 11059200         //晶振频率11.0592M；

sbit KEY_F_UP=P2^0;             //频率上调按钮；
sbit KEY_F_DOWN=P2^1;           //频率下调按钮；
sbit KEY_D_UP=P2^2;             //脉宽上调按钮；
sbit KEY_D_DOWN=P2^3;           //脉宽下调按钮；

sbit PWM_OUT=P1^0;               //波形输出IO;

extern unsigned char g_duty;             //初始占空比 50%
extern unsigned int  g_freq;           //初始频率 1KHz
extern unsigned char g_flag_f,g_flag_d; //频率调节标志和脉宽调制标志；

void calculate_f(unsigned int freq);
void calculate_d(unsigned int freq,unsigned int duty);
void pwm_init(void);
void key_scan(void);
void key_handle(void);


#endif

