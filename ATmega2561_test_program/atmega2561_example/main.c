#include <stdio.h>
#include <string.h>
#include <atmel_start.h>
#include <usart_basic.h>
#include <atomic.h>
#include "uart.h"

#define BAUD 38400

typedef void (*callback)(char* cmd);

typedef struct uart_cmd {
	const char* cmd;
	callback cb;
} UART_CMD_T;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	UART_CMD_T cmd_lut[] =
		{{"dio_high",printf},
		{"dio_low",printf},
		{"dio_read",(callback)printf},
		{"aio_read",(callback)printf}};
			
	char input_string[BUFFER_SIZE];
	
	USART_0_enable();
	ENABLE_INTERRUPTS();

	uint8_t max_i = sizeof(cmd_lut) / sizeof(UART_CMD_T);

	while (1) 
	{
		if(BufferPeekLast() == 0xA)	// if the last char was line feed
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
