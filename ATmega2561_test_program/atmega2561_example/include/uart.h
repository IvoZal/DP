/**
 * \file
 *
 * \brief UART higher level handler.
 *
 * Created: 06.02.2024 19:51:46
 *  Author: Ivo Zaludek
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include "usart_basic.h"

/* UART buffer size. */
#define BUFFER_SIZE 24

/* UART buffer. */
static volatile uint8_t u8Buffer[BUFFER_SIZE] = {0};
static volatile uint8_t u8Head = 0;
static volatile uint8_t u8Tail = 0;

/* Function puts u8Data into u8Buffer on position u8Tail. */
void BufferPut(uint8_t u8Data);

/* Function returns value in head position from u8Buffer. */
uint8_t BufferGet();

/* Function returns length of data saved in u8Buffer. */
uint8_t BufferLength();

/* Function return last value from u8Buffer. If buffer is empty, function returns 0. */
uint8_t BufferPeekLast();

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

/* Interrupt routine which saves received data to u8RXData buffer. */
ISR(USART0_RX_vect);

#endif /* UART_H_ */