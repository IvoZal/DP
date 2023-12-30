/*
 * uart.h - library for UART communication
 *
 * Created: 05.12.2023 23:43:56
 * Author: Ivo Zaludek
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/sfr_defs.h>

/* UART buffer size. */
#define BUFFER_SIZE 24

/* UART buffer. */
static volatile uint8_t u8Buffer[BUFFER_SIZE] = {0};
static volatile uint8_t u8Head = 0;
static volatile uint8_t u8Tail = 0;

/* Function returns value in head position from u8Buffer. */
uint8_t BufferGet();

/* Function returns length of data saved in u8Buffer. */
uint8_t BufferLength();

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

/* USART initialization, call before using USART */
void USART_Init(unsigned int iBaudrate);

/* Interrupt routine which saves received data to u8RXData buffer. */
ISR(USART_RX_vect);

#endif /* UART_H_ */