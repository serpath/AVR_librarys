#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define F_CPU 8000000UL

#define BAUDRATE 31250
#define UBRRVAL (((F_CPU / (BAUDRATE * 16UL))) - 1)

int counter = 0;
volatile unsigned char kurz = 0;
volatile unsigned char TEMP;
volatile unsigned char midi_data;
volatile unsigned char buffer1;
volatile char STATE= 0;
uint8_t einheit = 0;
uint8_t quarter = 0;


void TIMER_init()
{
	/************************* TIMER 0 ***************************************/

	TCCR0B	|=	(1<<CS01);		// Timer 0 Prescaled Clock
	TIMSK0	|=	(1<<TOIE0);		// Timer0 Interrupt erlauben
	TIFR0	|=	(1<<TOV0);		// Timer-Interrupt zur initialisierung löschen
	TCNT0=0;					// Starwert setzen
}
	

void MIDI()
{
	if(midi_data == 0b11111010)
	{
		//PORTB ^= (1<<PB0);
		counter =0;
		TEMP ^= (1<<0);
	}
	if(midi_data == 0b11111100)
	{
		//PORTB ^= (1<<PB0);
		counter =0;
		TEMP ^= (1<<1);
	}
	
	
	if(midi_data == 0b11111000)
	{	TEMP ^= (1<<2);
		counter++;
		if(counter == 5){
		PORTB |= (1<<PB1);}
		
		if(counter == 19){
		PORTB &= ~(1<<PB1);}
		
		if (counter == 24){
		counter = 0;}
	}
	
}



void USART_Init()
{	
	UBRR0L =  UBRRVAL;
	UBRR0H = (UBRRVAL>>8);
	
	
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B = (1<<RXEN0)| (1<<TXEN0) | (1<<RXCIE0);
}

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

void USART_transmit(uint8_t data)
{
	while( !( UCSR0A & (1<<UDRE0)));	//USART dataregister nicht leer
	UDR0 = data;						//UDR bekommt inhalt der variable data
}

int main()
{
	DDRD &= ~(1<<PD0);
	PORTD |= (1<<PD0);
	DDRB |= (1<<PB0) | (1<<PB1);
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
	DDRC &= ~(1<<PC3);
	PORTC |= (1<<PC3);
		
	USART_Init();
	SPI_init();
	TIMER_init();
	
	
	
	for(;;)
	{	
		sei();
		
		MIDI();
		buffer1 = TEMP;
		SPI_send(buffer1);
		buffer1 = 0x00;
		PORTC = 0x00;
		if(!(PINC & (1<<PC3))){USART_transmit(0b11111010);}
			
		PORTC++;
		if(!(PINC & (1<<PC3))){USART_transmit(0b11111100);PORTB ^= (1<<PB1);}
		
	}
}

ISR(USART_RX_vect)
{
	midi_data = UDR0;
	if(midi_data == 0b11111000)
	{	//TEMP ^= (1<<2);
		counter++;
		if(counter == 5){
		PORTB |= (1<<PB1);}
		
		if(counter == 19){
		PORTB &= ~(1<<PB1);}
		
		if (counter == 24){
		counter = 0;}
	}
}

ISR(TIMER0_OVF_vect)
{	
		
	
		if(einheit==0)
		{
			USART_transmit(0b11111000);
			quarter++;
			if(quarter>24){quarter=0;}
		}
		einheit++;
		if(einheit == 82){einheit = 0;}
		if(quarter == 23){PORTB |= (1<<PB1);}
		else{PORTB &= ~(1<<PB1);}
		
	
}