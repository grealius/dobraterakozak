
#include <stdio.h>
#include <string.h>

int main(void)
{

	char tab[100];

	fgets(tab, 21, stdin);
	
	printf("%s", tab);

	int cyfry=0;
	int i;

	for (i=0; i<21; i++)
		if (tab[i]>='0' && tab[i]<='9')
			cyfry++;

	printf("%d\n", cyfry);


	return 0;
}
