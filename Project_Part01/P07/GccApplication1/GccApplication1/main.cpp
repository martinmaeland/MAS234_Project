/*
 * GccApplication1.cpp
 *
 * Created: 16.10.2020 09:41:52
 * Author : marti
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>


int ADCsingleREAD(uint8_t adctouse)
{
	int ADCval;

	ADMUX = adctouse;         // use #adctouse ADC
	ADMUX |= (1 << REFS0);    // use AVcc as the reference
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	ADCSRA |= (1 << ADEN);    // Enable the ADC

	ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	while(ADCSRA & (1 << ADSC));      // Thanks T, this line waits for the ADC to finish


	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	return ADCval;
}

int main(void)
{
	DDRC = 0x00;
	DDRC |= (1<<PC5);
	PINC = 0x00;
	
    while (1) 
    {
		
		int ADCValue = ADCsingleREAD(0);	
		
		if (ADCValue > 200)
		{
			PORTC = 0b00100000;
		}
		else if (ADCValue < 400)
		{
			PORTC = 0x00;
		}
			
		/*		
		if (PINC & 0b00010000)
		{
			PORTC ^= (1 << PC5);
			_delay_ms(500);
		}
		*/
	
			
    }
}

