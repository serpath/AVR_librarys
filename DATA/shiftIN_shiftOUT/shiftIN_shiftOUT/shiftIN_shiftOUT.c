///////////////////////////////////////////// INCLUDES UND DEFINES ///////////////////////////////////

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "Timer.h"
#include "pinInt.h"
#include "SPI_init.h"
#include "LCD.h"

#define BIT(a) (1<<(a))
// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40
#define lcd_SetCursor       0b10000000          // set cursor position
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'

//////////////////////////////////////////////////////////////////////////////////////////////////////


void SWITCH();
void BerechneCnt();
void DAC_send(uint16_t data,uint8_t channel);

/////////////////////////////////////////////// globale Variablen ////////////////////////////////////

volatile uint8_t	ON					= 0;
		 uint16_t	Drehwert			= 120;
		 uint8_t	Drehencoder			= 0;
volatile uint8_t	SHIFT_buffer1		= 0;
volatile uint8_t	SHIFT_buffer2		= 0;
volatile uint8_t	TEMP_1				= 0x00;
volatile uint16_t	cnt					= 0;
volatile uint16_t	cnt_MAX				= 0;
volatile uint8_t	PrevInput1			= 0;
volatile uint8_t	PrevInput2			= 0;
volatile uint8_t	a					= 0x01;
		 //uint16_t	SWITCH_pressed		= 0;
volatile uint8_t	milo				= 0;
volatile uint8_t	BIT_checker			= 0;	
volatile uint8_t	active				= 0;
volatile uint16_t	CV_array_1[8]		= {4096,1300,0,2000,20,200,2000,3000};
volatile uint16_t	CV_array_2[8]		= {100,2300,4060,1,300,4090,10,1200};
volatile unsigned char	PATTERN[8]			= {		0b11010010,
												0b10100100,
												0b10001001,
												0b11111111,
												0b00011000,
												0b01010101,
												0b01111010,
												0b00000001
											};


uint8_t BPM [] = "BPM";
uint8_t MENU [] = "HAUPTMENU";

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void BLABLA(uint8_t catastrophe [20],uint16_t WERT)
{
	char str[16];
	uint8_t strong [16];
	
	
	itoa(WERT, str, 16);
	
	for(int i = 0; i<17;i++){strong[i] = str[i];}
	
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(80);
	//lcd_write_instruction_4d(lcd_Clear);
	lcd_write_string_4d(catastrophe);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(80);                                  // 40 uS delay (min)
	//lcd_write_instruction_4d(lcd_Clear);
	lcd_write_string_4d(strong);
}

void LCD_delete()
{
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(80);
	lcd_write_instruction_4d(lcd_Clear);
	
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(80);                                  // 40 uS delay (min)
	lcd_write_instruction_4d(lcd_Clear);
	
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(80);
}

int main()
{	
	
	DDRD |= (1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	DDRB |= (1<<PB1);
	EIMSK |= (1<<INT0);
	
	SPI_init();
	TIMER_init();
	pinInt();
	BerechneCnt();
	LCD_LCD();
	
	
	
	sei();
	
	uint16_t resetter1	= 0;
	

	char bla = 0;
	while(1)
	{	
			resetter1++;
			
		
			if(resetter1 == 50000)
			{	
				
				resetter1 = 0;LCD_delete();
				//PORTB ^= (1<<PB1);
				
			}
		
		SWITCH();
		
				if(a == 0x01)
				{
					bla++;
					if(bla==8)
					{
					milo++;
					if(milo>7){milo = 0;}
					bla=0;
					}
				}
	}
	
}

///////////////////////////////////////////////
ISR(INT0_vect)
{
	uint8_t Flanken = active & (PrevInput2);
	PrevInput2 = active;
	ON ^= Flanken;
	ON = !ON;
	TCNT0 = 250;
	
	PORTB ^= (1<<PB1);
	
	cnt=0;
	
}

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
				if (Drehwert < 4095) {Drehwert++;}
					BerechneCnt();
			}
			break;
		case 0x01:
			if ((AktuellePin & BIT(PC4))) {Drehencoder = 3;}
			else if ((AktuellePin & BIT(PC5)) == 0)
			{
				Drehencoder = 0;
				if (Drehwert > 0) {Drehwert--;}
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
	
	BLABLA(BPM,Drehwert);
	
	
	
}
//////////////////////////////////////////////////////
/////////////////////////////////////////////// Funktionen //////////////////////////////////////////

void BerechneCnt()
{
	cnt_MAX = (uint16_t)(468750UL / Drehwert);
}

void DAC_send(uint16_t data,uint8_t channel)
{
		uint16_t DAC_komplett =0b1101000000000000;
		switch (channel)
		if(channel == 1){DAC_komplett |= (1<<15);}
		if(channel == 2){DAC_komplett &= ~(1<<15);}
		DAC_komplett |= data;
		uint8_t DAC_H = 0;
		uint8_t DAC_L = 0;		
			
			DAC_komplett += data;
			DAC_L = DAC_komplett ;
			DAC_H = DAC_komplett >> 8;		
			
			SPI_send(DAC_H,3,0);
			SPI_send(DAC_L,3,1);
			
		
		
}
void SWITCH()
{
	SHIFT_buffer1 = TEMP_1;
		
	SPI_send((SHIFT_buffer1 ^ a),1,0);
	SHIFT_buffer2 = ~(SPDR);
	uint8_t Flanken = SHIFT_buffer2 & (~PrevInput1);
	PrevInput1 = SHIFT_buffer2;

	TEMP_1 ^= Flanken;
	TEMP_1 = PATTERN[milo];
	//if((TEMP_1 & (1<<BIT_checker)))	{PORTB |= (1<<PB1);_delay_us(100);PORTB &= ~(1<<PB1);}
	//else{PORTB &= ~(1<<PB1);}
	
	
	
		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_OVF_vect)
{
	//PORTB &= ~(1<<PB1);
	if(!ON){a = 0x01;}
	if(ON)
	{		
		if(cnt == 0)
		{
			PORTB &= ~(1<<PB1);		
			a <<= 1;
			if(a==0){a = 0x01;}	
				DAC_send(CV_array_1[BIT_checker],1);
				DAC_send(CV_array_2[BIT_checker],2);	
				
			if(SHIFT_buffer1 & (1<<BIT_checker))	{	PORTB |= (1<<PB1);	}	
				else {PORTB &= ~(1<<PB1);}
			BIT_checker++;
			if(BIT_checker > 7){BIT_checker = 0;}			
		}
		cnt++;
		if(cnt > cnt_MAX){cnt = 0;}		
	}
}