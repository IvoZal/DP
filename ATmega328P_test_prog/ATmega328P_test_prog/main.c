/*
 * ATmega328P_test_prog.c
 *
 * Created: 05.12.2023 19:52:06
 * Author : ACER_E15
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include "dio.h"
#include "adc.h"
#include "uart.h"

typedef void (*callback)(char* cmd);

typedef struct uart_cmd {
	const char* cmd;
	callback cb;
} UART_CMD_T;


int main(void)
{
    UART_CMD_T cmd_lut[] = 
		{{"dio_high",dio_write_high},
		{"dio_low",dio_write_low},
		{"dio_read",(callback)dio_read},
		{"aio_read",(callback)adc_read}};
	
	char input_string[BUFFER_SIZE];

	adc_init();
	USART_Init(38400);
	sei();
	
	uint8_t max_i = sizeof(cmd_lut) / sizeof(UART_CMD_T);
	
    while (1) 
    {
		if(BufferPeekLast() == 0xA)
		{
			uint8_t i = 0;
			while(BufferLength() > 0) 
			{
				input_string[i] = BufferGet();
				++i;
			}
			for(i=0; i < max_i; i++)
			{
				if(strstr(input_string,cmd_lut[i].cmd) != NULL)
				{
					cmd_lut[i].cb(input_string);
					break;
				}
			}
			if(i == max_i)
			{
				printf("ERROR: Unsupported command!\n");
			}
		}		
    }
}