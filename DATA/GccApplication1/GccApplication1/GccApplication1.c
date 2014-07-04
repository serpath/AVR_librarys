/*
 * GccApplication1.c
 *
 * Created: 19.06.2014 01:03:31
 *  Author: serpath
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>





volatile unsigned int i = 0;
volatile unsigned char ACT = 0;	//zählt schritte/kanäle durch
//volatile unsigned char STEP[8]; //={ 1,0,1,0,0,1,0,0								};
//volatile unsigned int minute;
//volatile unsigned int stunde;




void TIMER_init(void)
	{
		
	//TCCR0A = 0b00000000;
	TCCR2B |=  (1<<CS01) | (1<<CS00);//Prescaler von 64
	TIMSK2 |= (1<<TOIE2);//Overflow Interrupt Enable, bei Overflow wird interrupt ausgelöst
	TCNT2=255; //Timer/Counter Interrupt Register, Timer Overflow Flag gesetzt
	
	sei();

	}
	



void PORT_init(void){
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) ;	
	}

void ADC_start()
	{

	ADCSRA|= (1<<ADIF);//1 wird reingeschrieben um ADIF frei zu machen
	ADCSRA|= (1<<ADSC);//start convertierung

	}

void ADC_init(void){
	// A/D Conversion
	//DDRC &= ~((1<<PC0)|(1<<PC1)); //falls ausro werte falsch convertiert

	//ADEN: AD enabed, ADPS2,ADPS1: Prescale (Teilungsverhältnis)
	ADCSRA|= (1<<ADEN)|(1<< ADPS2)|(1<<ADPS1)|(1<<ADLAR);
	//anschalten / teilungsverhaeltnis 64 110
	}
unsigned char ADC_convert(unsigned char mux){
	
		ADMUX= (1<<REFS0)|(1<<ADLAR)|(mux);	
			ADC_start();
				while(!(ADCSRA & (1<<ADIF))){} //es wird geprüft ob ADIF frei
			return ADCH; //ADCH hier steht das ergebnis der Konvertierung drin
			}
	
void wait(volatile unsigned long time1,volatile unsigned long time2){
	for(int i = 0; i < time1; i++){}
		for(int i = 0; i < time2; i++){}
	}

unsigned char STEP(unsigned char channel){
	cli();
	switch (channel)
		{
		case 0: PORTB = 0b00000001;
			break;
		case 1: PORTB = 0b00000010;
			break;
		}	
	
	 return channel;
	}

int main(void)
{	
	PORT_init();
	ADC_init();
	TIMER_init();
	
	//unsigned char mom;
	
	
		while(1)
		{
			/*
			PORTB ^= (1<<PB1);
			wait(20000,20000);
			PORTB ^= (1<<PB0);
			wait(20000,20000);
			*/
				if(i == 10)
					{				
						PORTB ^= (1<<PB2);
						i=0;
					}
		
		
		
		}        
	}

ISR (TIMER2_OVF_vect)
	{
			
		i++;
	
						//zähle i hoch
	
		/*	
		if(i == 1000){		//i gleich tausend
						
			ACT++;			//zähle ACT hoch
				if(ACT > 1)	//1 = grenze vor überlauf
				{ACT = 0;}
			i = 0;
			}
	*/
	}