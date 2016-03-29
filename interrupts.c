// Brent Rubell and David Austin
// Lab: ADC + 7 Segment Display + Interrupts

// libs
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
	const uint8_t cathodelut[4] PROGMEM = { 0x01, 0x02, 0x08, 0x04};
	const uint8_t binlut[15] PROGMEM ={
		0b00111111, /* 0 */
		0b00000110, /* 1 */
		0b01010011, /* 2 */
		0b01001111, /* 3 */
		0b01100110, /* 4 */
		0b01101101, /* 5 */
		0b01111101, /* 6 */
		0b00000111, /* 7 */
		0b01111111, /* 8 */
		0b01100111, /* 9 */
		0b01110111, /* 10 */
		0b01111110, /* A */
		0b00111001, /* B */
		0b01011110, /* C */
		0b01111001, /* D */
		0b01110001  /* E */

	};
volatile uint8_t segupdate = 0;
volatile uint8_t adcupdate = 0;
	
int main(void)
{
	 uint16_t myVal = ADC;
	// 7-seg code inits
	int printnum = 4; // qty of #'s on 7seg


	uint8_t currentDigit; // counts from 0=>3, tells which cathode you're supposed to drive. grab individual decimal digits of myval based off of this. 
	// setup timer @ 240 hz
	// ctc mode
	// clk div / 1024
	// timer compare Init.
	// OCR0A
	TCCR0A |= (1<<WGM01) | (1<<CS02) | (1<<CS00);
	TCNT0 = 0;
	
	// interrupt setup
	ADCSRA |= (1<<ADEN); // ADC enabled pg 249, bottom of page
	// 50kHz < clk freq < 200kHz (pg240)
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // prescaler w/128 division factor
	ADMUX |= (1<<REFS0); // uses the AVCC voltage
	ADCSRA |= (1<<ADIE); // bit write to 1, when sreg's "i" is set, the ADC conversion complete interrupt is enabled (pg250)


	// portb setup
	// pb7 => pcint7
	// pcint7 => pcint0
	DDRB &= ~(1<<DDB0); // clear the PB0
	// PB0 is now an input
	PORTB |= (1<<PORTB0); // turn on pullup
	//pb0 is now an input w/pull-up enabled
	PCICR |= (1<<PCIE0); // enable PCMSK0 scan
	PCMSK0 |= (1<<PCINT0); // trigger on state change

	sei(); // enable interrupts


	while(1)
	{
		PINC ^= _BV(1); // toggle the pinc
		// do nothing b/c interrupt is handling this instead
		if (segupdate == 1)
		{
			//update 7seg
		}
		else if(adcupdate == 1)
		{
			//update ADC
		}
	}
}




void printLUT(void)
{
	DDRB = cathodelut[0];
	PORTB = cathodelut[0];
	DDRD = binlut[0];
	PORTD = binlut[0];
}

ISR (ADC_vect)
{
	// do stuff with adc here
}
ISR (PCINT0_vect)
{
	//
	if((PINB &(1<<PINB0)) == 1)
	{
		// low to high pin change
	}
	else
	{
		// high to low pin change
	}
}


ISR(BADISR_vect)
{
	// handles unexpected interrupts
	// put a breakpoint here to catch unhandled interrupts
}
