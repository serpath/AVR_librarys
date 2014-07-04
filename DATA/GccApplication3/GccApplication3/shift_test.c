#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void SPI_init(void)
{
	DDRB = (1<<PB3) | (1<<PB5) | (1<<PB7);	 				// Set MOSI , SCK , and SS output
	SPCR = ( (1<<SPE)|(1<<MSTR) | (1<<SPR1) |(1<<SPR0));	// Enable SPI, Master, set clock rate fck/128
}

void SPI_send(uint8_t byte)
{
	
	SPDR = byte;					//Load byte to Data register
	while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete
	PORTB |= (1<<PB1);
	PORTB &=~{1<<PB1};
}


int main(void)
{
	
	DDRB |=  (1<<PB2);
	
	PORTB &= ~(1<<PB2);
	
	while(1)
	{	
			
			
			
	}
}

/*

	uint8_t bello =	0b00000000;/*
							0b00000001,
							0b00000010,
							0b00000100,
							0b00001000,
							0b00010000,
							0b00100000,
							0b01000000,
							0b10000000,
							0b01000000,
							0b00100000,
							0b00010000,
							0b00001000,
							0b00000100,
							0b00000010,
							0b00000001,
							
							};
							
							*/
*/