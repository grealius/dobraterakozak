
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

const uint8_t cyfry[3]={0x01,0x02,0x04};

uint8_t scankbd()
{
	uint8_t y=0;
	uint8_t x;
	while(y<4)		//S¹ 4 rzêdy
	{
		x=PORTB;
		x=(x | 0x0F) ^ (1<<y);	//Wybierz aktywny rz¹d, nie zmieniaj¹c stanu innych pinów portu
		PORTB=x;
		asm volatile ("nop");	//Synchronizator
		x=PINB;
		if((x & 0x70)!=0x70)	//Jakiœ klawisz by³ wciœniêty?
		{
			if((x & 0x10)==0) x=0;	//Jeœli tak, to z której kolumny?
			else if((x & 0x20)==0) x=1;
			else x=2;
			return x;
		}
		y++;
	}
	return 255;
}

int main()
{
	DDRD=0xFF;		//Wszystkie piny portu s¹ wyjœciem
	DDRB=0x0F;	//4 rzêdy s¹ wyjœciami, 3 kolumny s¹ wejœciami
	PORTB=0x7F;	//W³¹cz pull upy na kolumnach, ustaw rzêdy w stanie 1

	uint8_t kbd;
	while(1)
	{
		kbd=scankbd();
		if (kbd!=255)
			PORTD=cyfry[kbd];
	}
}
