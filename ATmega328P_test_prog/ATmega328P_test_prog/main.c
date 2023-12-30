/*
 * ATmega328P_test_prog.c
 *
 * Created: 05.12.2023 19:52:06
 * Author : ACER_E15
 */ 

#include <avr/io.h>
#include <string.h>
#include "dio.h"
#include "adc.h"
#include "uart.h"

typedef void (*callback)(void* params);

typedef struct uart_cmd {
	const char cmd;
	callback cb;
} UART_CMD_T;



int main(void)
{
    UART_CMD_T cmd_lut[] = 
		{{"dio_high",dio_write_high},
		{"dio_low",dio_write_low},
		{"dio_read",dio_read},
		{"aio_read",adc_read}};
	
	char * input_string[BUFFER_SIZE];

    while (1) 
    {
		if(BufferLength() >= 1) 
		{
			uint8_t j = 0;
			while(BufferLength() > 0) 
			{
				input_string[j] = BufferGet();
				j++;
			}
			for(uint8_t i=0; i < sizeof(cmd_lut) / sizeof(UART_CMD_T); i++)
			{
				if(strstr((const char *)input_string, cmd_lut[i].cmd) != NULL)
				{
					cmd_lut[i].cb((void *)input_string);
				}
			}
		}		
    }
}

