#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Timer.h"
#include "pinInt.h"
#include "SPI_init.h"


extern volatile uint16_t	cnt;
extern volatile uint16_t	cnt_MAX;
extern volatile uint8_t	a;

void TIMER_init()
{
	/************************* TIMER 1 ***************************************/

	TCCR0B	|=	(1<<CS00);		// Timer 0 Prescaled Clock
	TIMSK0	|=	(1<<TOIE0);		// Timer0 Interrupt erlauben
	TIFR0	|=	(1<<TOV0);		// Timer-Interrupt zur initialisierung löschen
	
	
}


