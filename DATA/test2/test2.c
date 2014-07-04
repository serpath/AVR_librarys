#define F_CPU 8000000
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h> 
#include <util/delay.h>


volatile uint8_t z =0;

void TIMER_init()
{
	/************************* TIMER 1 ***************************************/	

	TCCR0B	|=	(1<<CS01);		// Timer 0 Prescaled Clock
	TIMSK0	|=	(1<<TOIE0);		// Timer0 Interrupt erlauben
	TIFR0	|=	(1<<TOV0);		// Timer-Interrupt zur initialisierung löschen
	TCNT0=0;					// Starwert setzen
	
	
	/************************* TIMER 1 - PWM *********************************/
	

}



int main()
{

	TIMER_init();
	DDRB |= (1<<PB1)|(1<<PB4);
	

	
	


		
		while(1)
		{	
		
				
			
			
			
		}



}

ISR(TIMER0_OVF_vect)
{

	
		PORTB ^= (1<<PB1);
	
}
