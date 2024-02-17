/*
 * encoder.c
 *
 * Created: 19.03.2023 22:07:43
 *  Author: Ivo Zaludek
 */ 

#include "encoder.h"

#define A (1 << PE7)		// INT7
#define B (1 << PE6)		// INT6
#define BTN (1 << PE5)		// INT5

static volatile uint8_t u8State;
static volatile int8_t i8StateChange = 0;
volatile uint16_t u32TimeStamp;

ISR(INT7_vect)
{
	Encoder_ChangeState();		// Check encoder state
	if(i8StateChange == 4) {	// If encoder made 1 full pulse clockwise, increment i16CwCounter
		i16CwCounter++;
		i8StateChange = 0;
	}
	else if(i8StateChange == -4) {	// If encoder made 1 full pulse anti-clockwise, decrement i16CcwCounter
		i16CcwCounter++;
		i8StateChange = 0;
	}
}

ISR(INT6_vect)
{
	Encoder_ChangeState();		// Check encoder state
	if(i8StateChange == 4) {	// If encoder made 1 full pulse clockwise, increment i16CwCounter
		i16CwCounter++;
		i8StateChange = 0;
	}
	else if(i8StateChange == -4) {	// If encoder made 1 full pulse anti-clockwise, decrement i16CcwCounter
		i16CcwCounter++;
		i8StateChange = 0;
	}
}

ISR(INT5_vect)
{
	if((PINE & BTN) == 0) {		// if the input is low for 60ms, increase the counter
		_delay_ms(60);			// debounce
		if((PINE & BTN) == 0) {
			i16BtnCounter++;
		}
	}
}

/*ISR(INT5_vect)
{
	if(getTime() > u32TimeStamp) {
		if((PINE & BTN) == 0) {
			i16BtnCounter++;
		}
		u32TimeStamp = getTime() + 60000;
	}	
}*/

void Encoder_Init()
{
	EICRB |= (1 << ISC70);	// INT7 interrupt at any logical change
	EICRB |= (1 << ISC60);	// INT6 interrupt at any logical change
	EICRB |= (1 << ISC50);	// INT5 interrupt at any logical change
	// PORTE |= BTN;			// Set BTN pin as a pullup
	
	EIMSK = A | B | BTN;	// External interrupt enable
	
	Encoder_SetState();		// Set initial state
	
	sei();	// enable interrupts
}

int16_t Encoder_Cw_Pulse_Count()
{
	return i16CwCounter;
}

int16_t Encoder_Ccw_Pulse_Count()
{
	return i16CcwCounter;
}

int16_t Encoder_Btn_Count()
{
	return i16BtnCounter;
}

void Encoder_SetState()
{
	switch ((PINE & A) |(PINE & B))
	{
		case 0:
		u8State = 2;
		break;
		
		case A:
		u8State = 1;
		break;
		
		case B:
		u8State = 3;
		break;
		
		case (A | B):
		u8State = 4;
		break;
	}
}

void Encoder_ChangeState()
{
	switch (u8State)	// Implementation of state machine, states are changed based on pins A and B
	{
		case 1:
		if((PINE & B) != B) {
			u8State = 2;
			i8StateChange++;
		}
		else if((PINE & A) == A) {
			u8State = 4;
			i8StateChange--;
		}
		break;
		
		case 2:
		if((PINE & A) == A) {
			u8State = 3;
			i8StateChange++;
		}
		else if((PINE & B) == B) {
			u8State = 1;
			i8StateChange--;
		}
		break;
		
		case 3:
		if((PINE & B) == B) {
			u8State = 4;
			i8StateChange++;
		}
		else if((PINE & A) != A) {
			u8State = 2;
			i8StateChange--;
		}
		break;
		
		case 4:
		if((PINE & A) != A) {
			u8State = 1;
			i8StateChange++;
		}
		else if((PINE & B) != B) {
			u8State = 3;
			i8StateChange--;
		}
		break;
	}
}