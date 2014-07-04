
#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned long z = 0;

int main(void)
{
	TCCR0B	|=	(1<<CS00);
	TIMSK0	|=	(1<<TOIE0);
	TIFR0	|=	(1<<TOV0);
	TCNT0=255;
	
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2);
	
	sei();
    while(1)
    {	
		
        if(z==10)	{	PORTB ^= (1<<PB0);		}
		if(z==100)	{	PORTB ^= (1<<PB0);		}
		if(z==1000)	{	PORTB ^= (1<<PB0);z=0;	}
    }
}

ISR (TIMER0_OVF_vect){
	
	z++;
	}