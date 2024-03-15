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

/********** Function definitions **********/

/* Interrupt function for delay */
ISR(TIMER3_OVF_vect)
{
	++timer1overflowCount;
	TCNT3 = 0;		// null the counter
	TIFR3 = (1 << TOV3) | (1 << OCF3A);		// start counting
}


void TimerInit()
{
	PRR1 &= ~(1 << PRTIM3);
	TCNT3 = 0;		// sets the counter to zero
	TCCR3B |= (1 << CS31);
	TIFR3 = (1 << TOV3) | (1 << OCF3A);
	TIMSK3 |= (1 << TOIE3);		// TOV3 flag interrupt enable
	sei();		// enable interrupts
}


uint32_t getTime()
{
	uint32_t time = TCNT3;
	time += (uint32_t)timer1overflowCount << 16;
	return time >> 1;	// return time (bit shift compensates clk frequency)
}


void delay(uint32_t us)
{
	uint32_t timeStamp = getTime() + us;
	while(timeStamp > getTime()) {}		// wait until time specified by timeStamp is reached
}