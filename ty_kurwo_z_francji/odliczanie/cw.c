
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

static const uint8_t __flash CYFRY[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void main(void) __attribute__((noreturn));

void main(void)
{

	DDRB=0xFF;

	uint8_t x=0;

	while (1)
	{
		PORTB=CYFRY[x];
		if (x==9)
			x=0;
		x++;
		_delay_ms(1000);
	}
}
