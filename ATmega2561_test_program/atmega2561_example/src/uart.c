/*
 * uart.c
 *
 * Created: 02.03.2024 17:15:50
 *  Author: Ivo Zaludek
 */ 

#include "uart.h"

/************************************************************************/
/* Function definition                                                  */
/************************************************************************/


/******************************************************************
	UART 0
******************************************************************/
void USART0_init(unsigned int baudrate)
{
	/* Enable USART0 */
	PRR0 &= ~(1 << PRUSART0);
	
	unsigned int ubrr = 16000000/16/baudrate-1;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data, 1 stop bit */
	UCSR0C = (3<<UCSZ00);
	/* Set receive interrupt */
	UCSR0B |= (1 << RXCIE0);
	
	stdout = &UART_0_stream;
}

int uart0_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

FILE UART_0_stream = FDEV_SETUP_STREAM(uart0_putchar, NULL,_FDEV_SETUP_WRITE);

ISR(USART0_RX_vect)
{
	UART0_buf_put(UDR0);	// Put data from uart into UART0_Buffer
}

void UART0_buf_put(uint8_t data)
{
	UART0_Buffer[UART0_Tail] = data;
	UART0_Tail = (UART0_Tail + 1) % BUFFER_SIZE;
}

uint8_t UART0_buf_get()
{
	uint8_t data = UART0_Buffer[UART0_Head];
	UART0_Head = (UART0_Head + 1) % BUFFER_SIZE;
	return data;
}

uint8_t UART0_buf_length()
{
	if (UART0_Tail >= UART0_Head) {
		return (UART0_Tail - UART0_Head);
	}
	else {
		return (BUFFER_SIZE + UART0_Tail - UART0_Head);
	}
}

uint8_t UART0_buf_peek_head()
{
	if (UART0_Tail != UART0_Head)
	{
		uint8_t lastIndex = (UART0_Tail == 0) ? (BUFFER_SIZE - 1) : (UART0_Tail - 1);
		return UART0_Buffer[lastIndex];
	}
	else
		return 0;
}





/******************************************************************
	UART 1
******************************************************************/
void USART1_init(unsigned int baudrate)
{
	/* Enable USART1 */
	PRR1 &= ~(1 << PRUSART1);
	
	unsigned int ubrr = 16000000/16/baudrate-1;
	/*Set baud rate */
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set frame format: 8 data, 1 stop bit */
	UCSR1C = (3<<UCSZ10);
	/* Set receive interrupt */
	UCSR1B |= (1 << RXCIE1);
}

int uart1_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR1A, UDRE1);
	UDR1 = c;
	return 0;
}

FILE UART_1_stream = FDEV_SETUP_STREAM(uart1_putchar, NULL,_FDEV_SETUP_WRITE);

ISR(USART1_RX_vect)
{
	UART1_buf_put(UDR1);	// Put data from uart into UART0_Buffer
}

void UART1_buf_put(uint8_t data)
{
	UART1_Buffer[UART1_Tail] = data;
	UART1_Tail = (UART1_Tail + 1) % BUFFER_SIZE;
}

uint8_t UART1_buf_get()
{
	uint8_t data = UART1_Buffer[UART1_Head];
	UART1_Head = (UART1_Head + 1) % BUFFER_SIZE;
	return data;
}

uint8_t UART1_buf_length()
{
	if (UART1_Tail >= UART1_Head) {
		return (UART1_Tail - UART1_Head);
	}
	else {
		return (BUFFER_SIZE + UART1_Tail - UART1_Head);
	}
}

uint8_t UART1_buf_peek_head()
{
	if (UART1_Tail != UART1_Head)
	{
		uint8_t lastIndex = (UART1_Tail == 0) ? (BUFFER_SIZE - 1) : (UART1_Tail - 1);
		return UART1_Buffer[lastIndex];
	}
	else
		return 0;
}