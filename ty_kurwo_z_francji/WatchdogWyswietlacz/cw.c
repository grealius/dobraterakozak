
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/atomic.h>

static const uint8_t __flash DIGITS[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};


uint8_t Seconds(void)
{
	PORTB=0x00;
	uint8_t x=0x21;
	uint8_t oldkey=PINC&(_BV(PC2) | _BV(PC3) | _BV(PC4));
	uint8_t counters[2]={0};
	uint8_t stop;;
	uint8_t i;
	uint8_t key;

	key=PINC&_BV(PC2);
	
	if (((oldkey^key) & _BV(PC2)) && (key==0))
	{
		while (1)
		{
			key=PINC&_BV(PC2);
			if (((oldkey^key)&_BV(PC2)) && (key==0))
				break;

			if (counters[0]==0)
			{
				key=PINC&_BV(PC3);
		
				if (((oldkey^key)&_BV(PC3)) && (key==0))
				{
					if (x>0)
					{
						PORTB=DIGITS[x];
						x--;
					}
					counters[0]=200;
				}
				oldkey&=~_BV(PC3);
				oldkey|=key;
			}
	
			if (counters[1]==0)
			{
				key=PINC&_BV(PC4);
		
				if (((oldkey^key)&_BV(PC4)) && (key==0))
				{
					if (x<9)
					{
						PORTB=DIGITS[x];
						x++;
					}
					counters[1]=200;
				}
				oldkey&=~_BV(PC4);
				oldkey|=key;
			}

			stop=0;
			for (i=0; i<2; i++)
				if (counters[i])
				{
					counters[i]--;
					stop=1;
				}
	
			if (stop)
				_delay_ms(1);
		}	
	}
	
	if (x==8)
		x=0x20;
	else if (x==9)
		x=0x21;
	
	return x;
}	
	
void DisplayDigit(uint8_t value)
{
	uint8_t tmp=0x00;
	
	if (value<10)
		tmp=DIGITS[value];
		
	PORTB=tmp;
}
	

void TurnOffDog(void) __attribute__((naked)) __attribute__((section(".init3")));
void TurnOffDog(void)
{
	MCUSR=0;
	wdt_disable();
}	

void main(void) __attribute__((noreturn));

void main(void)
{
	DDRC=0x00;
	DDRB=0xFF;
	PORTC=0x1C;

	uint8_t c;
	uint8_t z=0;

	c=Seconds();
	WDTCSR=_BV(WDE) | c;
	
	cli();
	while (1)
	{
		DisplayDigit(z);
		z++;
	
		if (z>9)
			z=0;
		_delay_ms(1000);	
	}
}
