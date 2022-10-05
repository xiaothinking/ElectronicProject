#ifndef __LCD1602_H
#define __LCD1602_H

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit LCD_RS = P2^0;             
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

void delay(int ms);
bit lcd_busy();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_pos(uchar pos);
void lcd_init();
void lcd_ui_screen(void);

#endif

