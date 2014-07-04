#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB = (1<<PB0);
	
	//char a ;
	char b = 0b11100101;
	char i = 0b00000001;
		
		
		while(1)
		{
			//_delay_ms(1000);
			if( i & b)
			{
			PORTB |= (1<<PB0);
			}
			else
			{
			PORTB &= ~ (1<<PB0);
			}
			i = i << 1;
			if(i == 0b10000000){i=0x01;}
		}



}
