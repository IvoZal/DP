/**
 * \file
 *
 * \brief UART higher level handler.
 *
 * Created: 06.02.2024 19:49:25
 *  Author: Ivo Zaludek
 */ 

#include "uart.h"

int uart_putchar(char c, FILE *stream)
{
	USART_0_write(c);
	return 0;
}

ISR(USART0_RX_vect)
{
	BufferPut(USART_0_get_data());	// Put data from uart into u8Buffer
}

void BufferPut(uint8_t u8Data)
{
	u8Buffer[u8Tail] = u8Data;
	u8Tail = (u8Tail + 1) % BUFFER_SIZE;
}

uint8_t BufferGet()
{
	uint8_t u8Data = u8Buffer[u8Head];
	u8Head = (u8Head + 1) % BUFFER_SIZE;
	return u8Data;
}

uint8_t BufferLength()
{
	if (u8Tail >= u8Head) {
		return (u8Tail - u8Head);
	}
	else {
		return (BUFFER_SIZE + u8Tail - u8Head);
	}
}

uint8_t BufferPeekLast()
{
	if (u8Tail != u8Head)
	{
		uint8_t lastIndex = (u8Tail == 0) ? (BUFFER_SIZE - 1) : (u8Tail - 1);
		return u8Buffer[lastIndex];
	}
	else
	return 0;
}