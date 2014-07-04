#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Timer.h"
#include "pinInt.h"
#include "SPI_init.h"

void SPI_init(void)
{
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5) ;
	DDRC |= (1<<PC1)|(1<<PC2);
	DDRB &= ~(1<<PB4);
	PORTB &= ~(1<<PB4);
	PORTB |= (1<<PB2);
	SPCR =   (1<<SPE) | (1<<MSTR);
}
uint8_t SPI_send(uint8_t byte,uint8_t chip) //
{
	//PORTC &= ~(1<<PC2);
	//PORTC |= (1<<PC2);
	PORTC &= ~(1<<chip);
	
	SPDR = byte;
	
	while(!(SPSR & (1<<SPIF)));
	
	PORTC |=  (1<<chip);
	PORTC &= ~(1<<chip);
	PORTC &= ~(1<<PC2);
	PORTC |= (1<<PC2);
	
	return SPDR;
}

