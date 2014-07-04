/*
 * attiny841_test.c
 *
 * Created: 30.06.2014 13:17:53
 *  Author: serpath
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{	
	DDRB |= (1<<PB0);
	
	
    while(1)
    {
        PORTB ^= (1<<PB0);
		_delay_ms(100);
    }
}