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

uint8_t sub_str_cmp(const char* main_string, const char* substring) 
{
	uint8_t val = 1;
	while (*substring != '\0') {
		if (*main_string != *substring) {
			val = 0;  // Not equal
		}
		++main_string;
		++substring;
	}
	return val;  // Equal
}


int main(void)
{
    UART_CMD_T cmd_lut[] = 
		{{"dio_high",dio_write_high},
		{"dio_low",dio_write_low},
		{"dio_read",dio_read},
		{"aio_read",adc_read}};
	
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