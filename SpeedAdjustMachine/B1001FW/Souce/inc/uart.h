#ifndef __UART_H
#define __UART_H

#include <reg52.h>
#include "pwm.h"

void uart_init(unsigned int baud);
void send_one_byte(unsigned char txd);
void send_n_byte(unsigned char * s);
void uart_handle(void);

#endif

