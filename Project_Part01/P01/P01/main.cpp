/*
 * P01.cpp
 *
 * Created: 09.10.2020 10:03:14
 * Author : marti
 */ 

#include <avr/io.h>


int main(void)
{
	
	DDRB |= (1<<PB5);
	PORTB |= (1<<PB5);
	
	//PORTD &= ~(1 << PD0);
    
    while (1) 
    {
		
    }
}

