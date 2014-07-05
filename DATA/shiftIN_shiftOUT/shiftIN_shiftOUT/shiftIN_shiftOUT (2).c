///////////////////////////////////////////// INCLUDES UND DEFINES ///////////////////////////////////

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Timer.h"
#include "pinInt.h"
#include "SPI_init.h"

#define BIT(a) (1<<(a))

//////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t SPI_send(uint8_t byte,uint8_t chip);
void SWITCH();
void BerechneCnt();

/////////////////////////////////////////////// globale Variablen ////////////////////////////////////

		 uint16_t	Drehwert			= 120;
		 uint8_t	Drehencoder			= 0;
volatile uint8_t	SHIFT_buffer1		= 0;
volatile uint8_t	SHIFT_buffer2		= 0;
volatile uint8_t	TEMP_1				= 0x00;
volatile uint16_t	cnt					= 0;
volatile uint16_t	cnt_MAX				= 0;
volatile uint8_t	PrevInput			= 0;
volatile uint8_t	a					= 0x01;
		 //uint16_t	SWITCH_pressed		= 0;
volatile uint8_t	BIT_checker			= 0;	

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{	
	SPI_init();
	TIMER_init();
	pinInt();
	BerechneCnt();
	DDRB |= (1<<PB1);
	
	sei();
	
	
	
	while(1)
	{	
		
		SWITCH();
		//if(PORTB & (1<<PB1)){_delay_ms(8);PORTB &= ~(1<<PB1);}
					
	}
	
}

///////////////////////////////////////////////

ISR(PCINT1_vect)
{
	uint8_t AktuellePin = PINC & (BIT(PC4)|BIT(PC5));

	switch (Drehencoder)
	{
		default:
		case 0x00:
			if (AktuellePin & BIT(PC4)) {Drehencoder = 2;}
			else if (AktuellePin & BIT(PC5))
			{
				Drehencoder = 1;
				//if (Drehwert != 0xFF) {Drehwert++;}
				//if (Drehwert + 1 != 0x00) {Drehwert++;}
				if (Drehwert < 300) {Drehwert++;}
					BerechneCnt();
			}
			break;
		case 0x01:
			if ((AktuellePin & BIT(PC4))) {Drehencoder = 3;}
			else if ((AktuellePin & BIT(PC5)) == 0)
			{
				Drehencoder = 0;
				if (Drehwert > 30) {Drehwert--;}
					BerechneCnt();
			}
			break;
		case 0x02:
			if ((AktuellePin & BIT(PC4)) == 0) {Drehencoder = 0;}
			else if (AktuellePin & BIT(PC5)) {Drehencoder = 3;}
			break;
		case 0x03:
			if ((AktuellePin & BIT(PC4)) == 0) {Drehencoder = 1;}
			else if ((AktuellePin & BIT(PC5)) == 0) {Drehencoder = 2;}
			break;
	}
}
//////////////////////////////////////////////////////
/////////////////////////////////////////////// Funktionen //////////////////////////////////////////

void BerechneCnt()
{
	cnt_MAX = (uint16_t)(468750UL / Drehwert);
}
void SWITCH()
{
	SHIFT_buffer1 = TEMP_1;
	
	/*
	if(~(SPDR) & (1<<0))
	{	SWITCH_pressed++;
	if(SWITCH_pressed == 2000){PORTB |= (1<<PB1);}
	}
	*/
		if(TEMP_1 & (1<<BIT_checker)) 
		{
			PORTB |= (1<<PB1);
			
			
		}
		else{PORTB &= ~(1<<PB1);}
		
		if(BIT_checker>7){BIT_checker = 0;}
		
	SPI_send((SHIFT_buffer1 ^ a),1);
	SHIFT_buffer2 = ~(SPDR);
	uint8_t Flanken = SHIFT_buffer2 & (~PrevInput);
	PrevInput = SHIFT_buffer2;

	TEMP_1 ^= Flanken;
	
	
	
		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
	PORTB &= ~(1<<PB1);
	
	if(cnt == 0)
	{
		
		a <<= 1;
		if(a==0){a = 0x01;}
			
			/*
			if(TEMP_1 & (1<<BIT_checker)) {PORTB |= (1<<PB1);}
			else{PORTB &= ~(1<<PB1);}
			BIT_checker++;
			if(BIT_checker>7){BIT_checker = 0;}
			*/
			BIT_checker++;
			
	}
	cnt++;
	if(cnt > cnt_MAX){cnt = 0;}
	
}