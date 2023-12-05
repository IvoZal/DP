/*
 * keyboard.c
 *
 * Created: 16.03.2023 20:31:17
 *  Author: Ivo Zaludek
 */ 

#include "keyboard.h"

#define ALL_ROWS (Row1 | Row2 | Row3 | Row4)
#define ALL_COL (Col1 | Col2 | Col3 | Col4)

// Definition of button order
static char cButtons[] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};

// Previously pressed button
char cPrevBtn;

char KeyboardRead()
{
	// Pull-up enable on columns and rows
	DDRB &= ~ALL_ROWS;
	PORTB |= ALL_ROWS;
	PORTC |= ALL_COL;
	for (uint8_t i = 0; i < 5; i++)		// Repeat for every row
	{
		switch (i)			// Select row as output
		{
			// For each Row, set row "i" as an output and turn off pull-up on it.
			case 0x0:
			DDRB |= Row1;
			PORTB &= ~Row1;
			break;
				
			case 0x1:
			DDRB = (DDRB & ~ALL_ROWS) | Row2;
			PORTB = (PORTB | ALL_ROWS) & ~Row2;
			break;
				
			case 0x2:
			DDRB = (DDRB & ~ALL_ROWS) | Row3;
			PORTB = (PORTB | ALL_ROWS) & ~Row3;
			break;
				
			case 0x3:
			DDRB = (DDRB & ~ALL_ROWS) | Row4;
			PORTB = (PORTB | ALL_ROWS) & ~Row4;
			break;
		}
		switch (~PINC & ALL_COL)			// Read if any column is pressed down
		{
			case Col1:
			return cButtons[i*4];
				
			case Col2:
			return cButtons[i*4 + 1];
				
			case Col3:
			return cButtons[i*4 + 2];
			
			case Col4:
			return cButtons[i*4 + 3];
		}
	}			
	return 0x00;
}

char KeyboardRead_OncePressed()
{
	char cBtn = KeyboardRead();
	if(cPrevBtn != cBtn) {
		cPrevBtn = cBtn;
		return cBtn;
	}
	else {
		cPrevBtn = cBtn;
		return 0x00;	
	}
}