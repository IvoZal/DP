/*
 * uart.c
 *
 * Created: 16.03.2023 23:44:04
 *  Author: Ivo Zaludek
 */ 

#include "uart.h"

/************************************************************************/
/* Function definition                                                  */
/************************************************************************/
void USART_Init(unsigned int baudrate)
{
	unsigned int ubrr = 16000000/16/baudrate-1;
	/* Set transmitter as output, assign HIGH value */
	DDRD |= (1 << DDD1);
	PORTD |= (1 << PORTD1);
	/* Enable pull-up for receiver */
	PORTD |= (1 << PORTD0);
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data, 1 stop bit */
	UCSR0C = (3<<UCSZ00);
	/* Set receive interrupt */
	UCSR0B |= (1 << RXCIE0);
	stdout = &mystdout;
}

int uart_putchar(char c, FILE *stream)
{
	// if (c == '\n')
	// uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

ISR(USART_RX_vect)
{
	BufferPut(UDR0);	// Put data from uart into u8Buffer
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
	return (u8Tail - u8Head);
}