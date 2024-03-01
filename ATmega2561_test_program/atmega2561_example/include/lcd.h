/*
 * lcd.h - library for 4 bit communication with HD44780U lcd display
 *
 * Created: 09.03.2023 21:32:36
 *  Author: Ivo Zaludek
 *
 * This header file contains function and variable declarations for
 * the lcd.h library. These declarations are needed by any code
 * that uses the library, so this file should be included in all source
 * files that use the library.
 *
 * Note: This file should be included after any system or third-party
 * header files that are needed by the library.
 */

#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define DB4 (1 << PC3)
#define DB5 (1 << PC4)
#define DB6 (1 << PC5)
#define DB7 (1 << PC6)
#define RS (1 << PC7)
#define E (1 << PG2)
#define DB_MASK (DB4 | DB5 | DB6 | DB7)

/* Display initialization, call after power on. */
void lcd_Init();

/* Send only lower 4 bits of cmd command. Used for initialization. */
void lcd_SendCmd4b(uint8_t cmd);

/* Function sends 8 bit command cmd to display. */
void lcd_SendCmd(uint8_t cmd);

/* Function sends 8 bit character data to display. */
void lcd_WriteChar(char data);

/* Sets DDRAM address 0 in address counter. 
Also returns display from being shifted to original position */
void lcd_RetHome();

/* Sets DDRAM address. */
void lcd_SetCursor(uint8_t address);

/* Shifts display or cursor left or right. 
Negative "shift" shifts display abs(shift) times left, positive shifts right.
Set sc to true to shift display, false to shift cursor. */
void lcd_Shift(int8_t shift, bool sc);

/* Clear the display and returns to home position. */
void lcd_Clear();

/* Putchar function for stream print. */
int lcd_putchar(char c, FILE *stream);

/* Definition of stream display. */
static FILE display = FDEV_SETUP_STREAM(lcd_putchar, NULL,_FDEV_SETUP_WRITE);

#endif /* LCD_H_ */