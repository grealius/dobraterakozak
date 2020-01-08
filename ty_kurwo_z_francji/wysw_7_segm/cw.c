
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

static const uint8_t __flash CYFRY[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
const uint8_t dot = 0x80;
uint8_t counters[2];

void PrintNumber(uint8_t value);

void main(void) __attribute__((noreturn));

void main(void)
{

	DDRB=0xFF;
	DDRC=0x00;
	PORTC|=_BV(PC4) | _BV(PC5);
	uint8_t x=0;
	uint8_t oldkey=PINC & (_BV(PC4) | _BV(PC5));
	uint8_t key;
	uint8_t i;
	uint8_t przerwa;

	while (1)
	{
		PrintNumber(x);

		if (counters[0]==0)
		{
			key=PINC & _BV(PC4);

			if (((key^oldkey) & PINC) && (key==0))
			{
				if (x > 0)
					x--;
				counters[0]=200;
			}

			oldkey&=_BV(PC4);
			oldkey|=key;
		}
		if (counters[1]==0)
		{
			key=PINC & _BV(PC5);
			if (((key^oldkey) & PINC) && (key==0))
			{
				if (x < 9)
					x++;
				counters[1]=200;
			}

			oldkey&=_BV(PC5);
			oldkey|=key;
		}

		przerwa=0;
		for (i=0; i<2; i++)
		{
			if (counters[i])
			{
				counters[i]--;
				przerwa=1;
			}
		}
		if (przerwa)
			_delay_ms(1);

	}

}

void PrintNumber(uint8_t value)
{
	uint8_t temp=0x0;

	if ((value & 0x7F) < 10)
		temp=CYFRY[value&0x7F];

	if ((value & dot) == 0)
		temp|=dot;

	PORTB=temp;
}

