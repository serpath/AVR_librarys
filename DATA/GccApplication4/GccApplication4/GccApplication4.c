/*
 * GccApplication4.c
 *
 * Created: 21.06.2014 02:16:13
 *  Author: serpath
 */ 


#include <avr/io.h>

int main(void)
{	
	
	DDRB |=  (1<<PB0);
	//DDRD &= ~(1<<PD7);
	
	PORTB &= ~(1<<PB0);
	
    while(1)
    {
        if(!(PIND & 1<<PD7))
		{PORTB ^= (1<<PB0);} 
    }
}                  