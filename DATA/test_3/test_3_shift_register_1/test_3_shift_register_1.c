#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include <math.h>


#define DEBOUNCE_TIME  1000
volatile unsigned char a =0b00000000;
volatile unsigned char cnt = 0;
volatile unsigned char MULTI_channel = 0b00000000;
volatile unsigned char STATE = 0x00;
volatile uint8_t buffer1 = 0x01;


void SPI_init(void)
{
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB5) ;	 				// Set MOSI , SCK , and SS output
	SPCR = (1<<SPE) | (1<<MSTR);	// Enable SPI, Master, set clock rate fck/128
}

void SPI_send(uint8_t byte) // sendet daten an SPI out und CLOCKED es kurz zum latchen
{
	PORTB &= ~(1<<PB0);				//LATCH low, ermöglicht SPI transfer zum slave
	SPDR = byte;					//Load byte to Data register
	while(!(SPSR & (1<<SPIF))); 	// Wait for transmission complete
	PORTB |=  (1<<PB0);
	PORTB &= ~(1<<PB0);
}

uint8_t debounce(void)
{
  if (bit_is_clear(PINC, PC3)) {      /* button is pressed now */
    _delay_us(DEBOUNCE_TIME);
    if (bit_is_clear(PINC, PC3)) {            /* still pressed */
      return (1);
    }
  }
  return (0);
}

unsigned char SWITCH_status()
{	
	
	
		for (MULTI_channel = 0b00000000; MULTI_channel<0b00001000;MULTI_channel++)
		{
			PORTC = 0x00;
			PORTC |= MULTI_channel;
			_delay_ms(1);

			
		}
		
	return STATE;

}




int main()
{
	
	SPI_init();

	DDRD = 0xff;
	DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2) ;
	DDRC &= ~((1<<PC3)| (1<<PC4) | (1<<PC5));
	PORTC |= ((1<<PC3)| (1<<PC4) | (1<<PC5));
	PCICR |= (1<<PCIE1);
	PCIFR |= (1<<PCIF1);
	PCMSK1 |= (1<<PCINT12) | (1<<PCINT13);
	/*
	TCCR0B	|=	(1<<CS02);		// Timer 0 Prescaled Clock
	TIMSK0	|=	(1<<TOIE0);		// Timer0 Interrupt erlauben
	TIFR0	|=	(1<<TOV0);		// Timer-Interrupt zur initialisierung löschen
	//TCNT0=255;					// Starwert setzen
	*/


	 
	//unsigned char buffer2 = 0x00;
	sei();
	
	while(1)
	{		
		
				
					
	
		
			
		SPI_send(buffer1);
		
		//_delay_ms(70);
		
			
	}

}

ISR(TIMER0_OVF_vect)
{
	
	cli();
		if(cnt == 0)
		{
	
			a *=2;
			if(a==0){a = 1;}
			a &= ~(1<<(PINC)); 
		
		}
		cnt++;
		if(cnt > 15){cnt = 0;}
	sei();
}





ISR(PCINT1_vect) 
{	
	cli();
	cnt++;
	if(cnt==40)
	{
		if(!(PINC & (1<<PC3)))
				{if((buffer1++) != (0xFF))
					{buffer1*2;}}
		
			if(!(PINC & (1<<PC4)))
				{if((buffer1--) != (0x00))
					{buffer1/2;}}
	cnt=0;
	}	

	sei();
}

