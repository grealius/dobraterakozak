
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

static const uint8_t __flash DIGITS[11] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x81, 0x41, 0x21};

uint8_t ScanKeyboard(void);

void main(void) __attribute__((noreturn));

void main(void)
{
	DDRB=0x0F;
	PORTB=0xFF;
	DDRD=0xFF;

	uint8_t key;
	while (1)
	{
		key=ScanKeyboard();
		if (key!=255)
			PORTD=0x01;
		else
			PORTD=0x02;	
	}
}

uint8_t ScanKeyboard(void)
{
	uint8_t rows=0;
	uint8_t x;
	
	while (rows<4)
	{
		x=PORTB;
		x=(x | 0x0F) ^ (1<<rows);
		PORTB=x;
		asm volatile("nop");
		x=PINB;
		if ((x&0xF0) != 0xF0)
		{
			if ((x&0x10)==0)
				x=0;
			else if ((x&0x20)==0)
				x=1;
			else if ((x&0x40)==0)
				x=2;
			else
				x=3;
			return x;
		}
		rows++;
	}
	return 255;
}	

