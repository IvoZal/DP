/*
 * keyboard.c
 *
 * Created: 16.03.2023 20:31:17
 *  Author: Ivo Zaludek
 */ 

#include "keyboard.h"

// Definition of button order
char cButtons[] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};

uint8_t KeyboardReadIdx()
{
	// Pull-up enable on columns and rows
	DDRA &= ~ALL_ROWS;
	PORTA |= ALL_ROWS;
	PORTA |= ALL_COL;
	for (uint8_t i = 0; i < 5; i++)		// Repeat for every row
	{
		switch (i)			// Select row as output
		{
			// For each Row, set row "i" as an output and turn off pull-up on it.
			case 0x0:
				DDRA |= Row1;
				PORTA &= ~Row1;
				break;
				
			case 0x1:
				DDRA = (DDRB & ~ALL_ROWS) | Row2;
				PORTA = (PORTB | ALL_ROWS) & ~Row2;
				break;
				
			case 0x2:
				DDRA = (DDRB & ~ALL_ROWS) | Row3;
				PORTA = (PORTB | ALL_ROWS) & ~Row3;
				break;
				
			case 0x3:
				DDRA = (DDRB & ~ALL_ROWS) | Row4;
				PORTA = (PORTB | ALL_ROWS) & ~Row4;
				break;
		}
		switch (~PINA & ALL_COL)			// Read if any column is pressed down
		{
			case Col1:
				return i*4;
				
			case Col2:
				return i*4 + 1;
				
			case Col3:
				return i*4 + 2;
			
			case Col4:
				return i*4 + 3;
		}
	}			
	return 0xFF;
}

char KeyboardRead()
{
	uint8_t idx = KeyboardReadIdx();
	if(idx != 0xFF)
		return cButtons[idx];
	return 0;
}