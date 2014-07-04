#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Timer.h"
#include "pinInt.h"
#include "SPI_init.h"



void pinInt()
{
	PCICR |= (1<<PCIE1);
	PCIFR |= (1<<PCIF1);
	PCMSK1 |= (1<<PCINT12)|(1<<PCINT13);	//PC5 und PC4 
}
