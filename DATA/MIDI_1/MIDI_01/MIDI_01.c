#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define F_CPU 8000000UL

#define BAUDRATE 31250
#define UBRRVAL (((F_CPU / (BAUDRATE * 8UL))) - 1)

int counter = 0;
volatile char kurz = 0;

ISR(USART_RX_vect)
{
 	kurz++;
char midi_data;
	
   midi_data = UDR0;
   if(midi_data == 0xF8){
      counter++;
      if(counter == 5){
         
      }
      if (counter == 24){
         counter = 0;
         
      }
   }
   if(kurz ==100){PORTB ^= (1<<PB1); kurz = 0;}
}


void USART_Init(){   
                                          
   UBRR0L=UBRRVAL;                              
   UBRR0H=(UBRRVAL>>8);                         
   UCSR0C =(1<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);   
   UCSR0B =(1<<RXEN0)|(1<<RXCIE0);               
   sei();                                 
}
   
int main()
{
   DDRD &= ~(1<<PD0);
   
   sei();


   DDRB = 0xFF;
   USART_Init();
   for(;;){
   }
} 
