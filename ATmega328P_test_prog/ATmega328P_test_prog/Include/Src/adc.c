/*
 * adc.c
 *
 * Created: 11.12.2023 21:39:42
 * Author: Ivo Zaludek
 */ 

#include "adc.h"

/********** Function declarations **********/
void adc_init()
{
	ADMUX |= (1 << REFS0);				// auto trigger in free running mode (ADTS = 0)
	ADCSRA = (1 << ADATE);				// auto trigger enable
	ADCSRA = (1 << ADEN) | (0b00000111);	// ADC enable | prescaler 128
}

uint16_t adc_read_from_ch(uint8_t channel)
{
	DIDR0 |= (1 << channel);		// digital input on pin "u8Channel" disable
	ADMUX = channel | (0xF0 & ADMUX);		// select "u8Channel"
	ADCSRA |= (1 << ADSC);			// start ADC
	while((ADCSRA & (1 << ADIF)) == 0) {}	// wait for result
	DIDR0 &= !(1 << channel);
	uint16_t conv_val = ADC;
	printf("ADC%u=%u\n", channel, conv_val);
	return conv_val;
}

uint16_t adc_read(char* cmd)
{
	uint8_t channel;
	if (cmd[12] > 0x29 && cmd[12] < 0x3A)	// numbers 0x0 - 0x9
		channel = cmd[12] - 0x30;
	else if (cmd[12] > 0x40 && cmd[12] < 0x47)	// numbers 0xA - 0xF
		channel = cmd[12] - 0x40 + 0xa;
	else {
		printf("ERROR: Invalid ADC channel!\n");
		return 0xFFFF;
	}
	return adc_read_from_ch(channel);
}