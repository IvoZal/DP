/*
 * lcd.c
 *
 * Created: 09.03.2023 21:32:49
 *  Author: Ivo Zaludek
 */ 

#include "lcd.h"

#define DB4 (1 << PC3)
#define DB5 (1 << PC4)
#define DB6 (1 << PC5)
#define DB7 (1 << PC6)
#define RS (1 << PC7)
#define E (1 << PG2)

/********** Function declarations **********/
void lcd_Init()
{
	/* Set all display pins as output */
	DDRC |= DB_MASK | RS;
	DDRG |= E;
	// Wait for 20 ms before first instruction
	delay(20000);
	// Function set
	lcd_SendCmd4b(0x03);
	delay(5000);
	// Function set
	lcd_SendCmd4b(0x03);
	// Function set
	lcd_SendCmd4b(0x03);
	// Function set
	lcd_SendCmd4b(0x02);
	// Function set + set 2 lines, font 5x8
	lcd_SendCmd(0b00101000);
	// Display off
	lcd_SendCmd(0b00001011);
	// Display clear
	lcd_SendCmd(0x01);
	delay(10000);
	// turn on display
	lcd_SendCmd(0b00001100);
	// Entry mode set, increment I/D = 1, shift S = 0
	lcd_SendCmd(0b00000110);
}

void lcd_SendCmd4b(uint8_t cmd)
{
	/* Set RS low */
	PORTC &= ~RS;
	/* Send data bits 0-4 */
	PORTC = (PORTC & ~DB_MASK) | ((cmd & 0x0F) << 3);
	/* Toggle E */
	PORTG |= E;
	delay(1);
	PORTG &= ~E;
	delay(200);
}

void lcd_SendCmd(uint8_t cmd)
{
	/* Set RS low */
	PORTC &= ~RS;
	// Send data bits 5-8
	PORTC = (PORTC & ~DB_MASK) | ((cmd & 0xF0) >> 1);
	/* Toggle E */
	PORTG |= E;
	delay(1);
	PORTG &= ~E;
	delay(50);
	/* Send data bits 0-4 */
	PORTC = (PORTC & ~DB_MASK) | ((cmd & 0x0F) << 3);
	/* Toggle E */
	PORTG |= E;
	delay(1);
	PORTG &= ~E;
	delay(200);
}

void lcd_WriteChar(char data)
{
	if (data == '°')	// conversion for extended ASCII
	{
		data = 0b11011111;
	}
	/* Set RS high */
	PORTC |= RS;
	/* Send data bits 5-8 */
	PORTC = (PORTC & ~DB_MASK) | ((data & 0xF0) >> 1);
	/* Toggle E */
	PORTG |= E;
	delay(1);
	PORTG &= ~E;
	delay(50);
	/* Send data bits 0-4 */
	PORTC = (PORTC & ~DB_MASK) | ((data & 0x0F) << 3);
	/* Toggle E */
	PORTG |= E;
	delay(1);
	PORTG &= ~E;
	delay(200);
}

void lcd_RetHome()
{
	lcd_SendCmd(0x02);
	delay(2000);
}

void lcd_SetCursor(uint8_t address)
{
	lcd_SendCmd(0x80 | address);
}

void lcd_Shift(int8_t shift, bool sc)
{
	uint8_t cmd;
	if(shift < 0) 
	{		
		cmd = 0x10 | (sc << 3); 
	}
	else 
	{
		cmd = 0x14 | (sc << 3);
	}
	for(uint8_t i = 0; i < abs(shift); i++)
	{
		lcd_SendCmd(cmd);
	}
}

void lcd_Clear()
{
	lcd_SendCmd(0x01);
	delay(1500);
}

int lcd_putchar(char c, FILE *stream)
{
	//if (c == '\n')
	//	lcd_putchar('\r', stream);
	lcd_WriteChar(c);
	return 0;
}

FILE display = FDEV_SETUP_STREAM(lcd_putchar, NULL,_FDEV_SETUP_WRITE);