#ifndef __PWM_H
#define __PWM_H

#include <reg52.h>
#include <math.h>
#include "lcd1602.h"

#define  F_osc 11059200         //����Ƶ��11.0592M��

sbit KEY_F_UP=P2^0;             //Ƶ���ϵ���ť��
sbit KEY_F_DOWN=P2^1;           //Ƶ���µ���ť��
sbit KEY_D_UP=P2^2;             //�����ϵ���ť��
sbit KEY_D_DOWN=P2^3;           //�����µ���ť��

sbit PWM_OUT=P1^0;               //�������IO;

extern unsigned char g_duty;             //��ʼռ�ձ� 50%
extern unsigned int  g_freq;           //��ʼƵ�� 1KHz
extern unsigned char g_flag_f,g_flag_d; //Ƶ�ʵ��ڱ�־��������Ʊ�־��

void calculate_f(unsigned int freq);
void calculate_d(unsigned int freq,unsigned int duty);
void pwm_init(void);
void key_scan(void);
void key_handle(void);


#endif

