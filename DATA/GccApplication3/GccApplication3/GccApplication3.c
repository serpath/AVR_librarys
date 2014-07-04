#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <math.h>


volatile unsigned char a;
volatile unsigned char cnt = 0;



void SPI_init(void)
{
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5) ;	 DDRC |= (1<<PC1)|(1<<PC2);	// Set MOSI , SCK , and SS output
	SPCR = (1<<SPE) | (1<<MSTR);	// Enable SPI, Master, set clock rate fck/128
}

void SPI_send(uint8_t byte) // sendet daten an SPI out und CLOCKED es kurz zum latchen
{
	PORTC &= ~(1<<PC1);				//LATCH low, ermöglicht SPI transfer zum slave
	SPDR = byte;					//Load byte to Data register
	while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete
	PORTC |=  (1<<PC1);
	PORTC &= ~(1<<PC1);
}

uint8_t  SPI_receive(void)
{	
	PORTC &= ~(1<<PC2);	
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	PORTC |=  (1<<PC2);	
	PORTC &= ~(1<<PC2);	
	/* Return Data Register */
	return SPDR;
}

int main()
{
	
	SPI_init();
	
	
	
	while(1)
	{
		
		a = SPI_receive();
		//a = SWITCH_status();
		//SPI_send(c);
		SPI_send(a);
		_delay_ms(70);
		/*
		a *= 2; ////////////
		if(a==0){a = 0x01;}

		*/
		
		
		
		
		
		
	}

}




