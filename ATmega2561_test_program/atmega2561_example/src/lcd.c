/*
 * lcd.c
 *
 * Created: 09.03.2023 21:32:49
 *  Author: Ivo Zaludek
 */ 

#include "lcd.h"

#define DB4 (1 << DDD4)
#define DB5 (1 << DDD5)
#define DB6 (1 << DDD6)
#define DB7 (1 << DDD7)
#define RS (1 << DDD3)
#define E (1 << DDD2)

/********** Function declarations **********/
void lcd_Init()
{
	/* Set all display pins as output */
	DDRD = DB4 | DB5 | DB6 | DB7 | RS | E;
	// Wait for 20 ms before first instruction
	_delay_us(20000);
	// Function set
	lcd_SendCmd4b(0x03);
	_delay_us(5000);
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
	_delay_us(10000);
	// turn on display
	lcd_SendCmd(0b00001100);
	// Entry mode set, increment I/D = 1, shift S = 0
	lcd_SendCmd(0b00000110);
}

void lcd_SendCmd4b(uint8_t cmd)
{
	/* Set RS low */
	PORTD &= ~RS;
	/* Send data bits 0-4 */
	PORTD = (PORTD & 0x0F) | (cmd << 4);
	/* Toggle E */
	PORTD |= E;
	_delay_us(1);
	PORTD &= ~E;
	_delay_us(200);
}

void lcd_SendCmd(uint8_t cmd)
{
	/* Set RS low */
	PORTD &= ~RS;
	// Send data bits 5-8
	PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
	/* Toggle E */
	PORTD |= E;
	_delay_us(1);
	PORTD &= ~E;
	_delay_us(50);
	/* Send data bits 0-4 */
	PORTD = (PORTD & 0x0F) | (cmd << 4);
	/* Toggle E */
	PORTD |= E;
	_delay_us(1);
	PORTD &= ~E;
	_delay_us(200);
}

void lcd_WriteChar(char data)
{
	if (data == '°')	// conversion for extended ASCII
	{
		data = 0b11011111;
	}
	/* Set RS high */
	PORTD |= RS;
	/* Send data bits 5-8 */
	PORTD = (PORTD & 0x0F) | (data & 0xF0);
	/* Toggle E */
	PORTD |= E;
	_delay_us(1);
	PORTD &= ~E;
	_delay_us(50);
	/* Send data bits 0-4 */
	PORTD = (PORTD & 0x0F) | (data << 4);
	/* Toggle E */
	PORTD |= E;
	_delay_us(1);
	PORTD &= ~E;
	_delay_us(200);
}

void lcd_RetHome()
{
	lcd_SendCmd(0x02);
	_delay_us(2000);
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
	_delay_us(1500);
}

int lcd_putchar(char c, FILE *stream)
{
	//if (c == '\n')
	//	lcd_putchar('\r', stream);
	lcd_WriteChar(c);
	return 0;
}