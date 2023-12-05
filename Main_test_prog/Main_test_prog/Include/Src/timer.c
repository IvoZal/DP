/*
 * timer.c
 *
 * Created: 25.02.2023 11:00:42
 *  Author: Ivo Zaludek
 */ 
#include "timer.h"

#define NOP() asm volatile ("nop")

/********** Variable declaration **********/
/* Counter for upper 16 bits of time returned by getTime function. */
static volatile uint16_t timer1overflowCount = 0;

/********** Function declarations **********/
void busyDelay(uint32_t us)
{
	/* time compensation */
	us -= 2;
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	/* 1 cycle takes 1 us */
	for (; us != 0; --us)
	{
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}
}

/* Interrupt function for delay */
ISR(TIMER1_OVF_vect)
{
	++timer1overflowCount;
	TCNT1 = 0;		// null the counter
	TIFR1 = (1 << TOV1) | (1 << OCF1A);		// start counting
}


void timerInit()
{
	sei();		// enable interrupts
	//timer1overflowCount = 0;	// sets the overflow counter to zero
	TCNT1 = 0;		// sets the counter to zero
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS12);
	TIFR1 = (1 << TOV1) | (1 << OCF1A);
	TIMSK1 |= (1 << TOIE1);		// TOV1 flag interrupt enable
}


uint32_t getTime()
{
	uint32_t time = TCNT1;
	time += (uint32_t)timer1overflowCount << 16;
	return time >> 1;	// return time (bit shift compensates clk frequency)
}


void delay(uint32_t us)
{
	uint32_t timeStamp = getTime() + us;
	while(timeStamp > getTime()) {}		// wait until time specified by timeStamp is reached
}