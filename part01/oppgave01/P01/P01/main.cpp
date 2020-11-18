/*
 * P01.cpp
 *
 * Created: 09.10.2020 10:03:14
 * Author : marti
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void sleep(int t)
{
	int c = 0;
	while(c < t)
	{
		_delay_ms(1);
		c++;
	}
}



int main(void)
{
	
	DDRC |= 1 << PC5;
	PORTC = 0x00;
	int pulseWidth = 5;
	double delay = 20;
	
	//PORTD &= ~(1 << PD0);
    
    while (1) 
    {
		
		for (int i=1; i<pulseWidth; i++)
		{
			for (int j=1; j<delay; j++)
			{
				sleep(1);
				PORTC ^= (1 << PC5);
				sleep(i);
				PORTC ^= (1 << PC5);
				sleep(pulseWidth-i);
			}
		}
		
		for (int i=pulseWidth; i>0; i--)
		{
			for (int j=1; j<delay; j++)
			{
				sleep(1);
				PORTC ^= (1 << PC5);
				sleep(i);
				PORTC ^= (1 << PC5);
				sleep(pulseWidth-i);
			}
		}
		
    }
}
