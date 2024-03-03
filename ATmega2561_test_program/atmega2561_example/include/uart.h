/*
 * uart.h - library for UART communication
 *
 * Created: 02.03.2024 17:13:56
 * Author: Ivo Zaludek
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

/* UART buffer size. */
#define BUFFER_SIZE 16

/******************************************************************
	UART 0
******************************************************************/
/* UART buffer. */
static volatile uint8_t UART0_Buffer[BUFFER_SIZE] = {0};
static volatile uint8_t UART0_Head = 0;
static volatile uint8_t UART0_Tail = 0;

/* Function puts data into Buffer on position Tail. */
void UART0_buf_put(uint8_t data);

/* Function returns value in head position from Buffer. */
uint8_t UART0_buf_get();

/* Function returns length of data saved in Buffer. */
uint8_t UART0_buf_length();

/* Function return last value from Buffer. If buffer is empty, function returns 0. */
uint8_t UART0_buf_peek_head();

int uart0_putchar(char c, FILE *stream);

extern FILE UART_0_stream;

/* USART initialization, call before using USART */
void USART0_init(unsigned int baudrate);

/* Interrupt routine which saves received data to RXData buffer. */
ISR(USART0_RX_vect);

/******************************************************************
	UART 1
******************************************************************/
/* UART buffer. */
static volatile uint8_t UART1_Buffer[BUFFER_SIZE] = {0};
static volatile uint8_t UART1_Head = 0;
static volatile uint8_t UART1_Tail = 0;

/* Function puts data into Buffer on position Tail. */
void UART1_buf_put(uint8_t data);

/* Function returns value in head position from Buffer. */
uint8_t UART1_buf_get();

/* Function returns length of data saved in Buffer. */
uint8_t UART1_buf_length();

/* Function return last value from Buffer. If buffer is empty, function returns 0. */
uint8_t UART1_buf_peek_head();

int uart1_putchar(char c, FILE *stream);

extern FILE UART_1_stream;

/* USART initialization, call before using USART */
void USART1_init(unsigned int baudrate);

/* Interrupt routine which saves received data to RXData buffer. */
ISR(USART1_RX_vect);

#endif /* UART_H_ */