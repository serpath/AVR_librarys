#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>


//Define functions
//======================
void ioinit(void);
void led_on(void);
void led_off(void);
//======================

int main (void)
{
	ioinit(); //Setup IO pins and defaults
	
	while (1)
	{
		int num_presses;                  //Number of times button has been iterations
		int num_nopress_iterations;       //Number of loop iterations since last button press
		char button_state;                //1=pressed, 0=not pressed
		
		//Wait for button to be pressed
		while (!bit_is_clear(PIND, 7)) {}
		num_presses=1;
		button_state=1;
		
		while (1)
		{
			if (bit_is_clear(PIND, 7)) //button is pressed
			{
				if (button_state==0) //was previously not pressed)
				{
					num_presses++;
				}
				button_state=1;
			}
			else //button is not pressed
			{
				if (button_state==1) //was previously pressed
				{
					num_nopress_iterations=0;
				}
				num_nopress_iterations++;
				button_state=0;
				if (num_nopress_iterations>20000) //If we haven’t seen a button press for a while, terminate the loop
				{
					break;
				}
			}
		}
		
		for (int i=0;i<num_presses;i++)
		{
			led_on();
			_delay_ms(300);
			led_off();
			_delay_ms(300);
		}
		
		if (bit_is_clear(PINC, 5))
		{
		}
	}
}


void ioinit (void)
{
	DDRD  &= (1<<PD7); //1 = output, 0 = input
	PORTD |= (1<<PD7); //Enable pin 5 internal pullup
	DDRB  |= (1<<PB0);
}

void led_on(void)
{
	PORTB |= _BV(PB0);
}

void led_off(void)
{
	PORTB &= ~_BV(PB0);
}

