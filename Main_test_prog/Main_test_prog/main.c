/*
 * Main_test_prog.c
 *
 * Created: 05.12.2023 21:35:24
 * Author : ACER_E15
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include "uart.h"

typedef void (*callback)(char* cmd);

typedef struct uart_cmd {
	const char* cmd;
	callback cb;
} UART_CMD_T;

int main(void)
{
    USART_Init(39400);
	sei();
	
    while (1) 
    {
    }
}

