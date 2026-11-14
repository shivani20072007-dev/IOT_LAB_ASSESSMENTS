#include<reg51.h>

void first()
{
	SCON = 0x50;
	TMOD9 = 0x20;
	TH1 = 0xFD;
	TL1 = 0xFD;
	TR1 = 1;
}

void transmit(char a)
{
	SBUF = a;
	while(TI==0);
	TI = 0;
}

void main()
{
	first();
	while(1)
	{
		char a[] = "wiurefbiukerybg\n";
		int b = 0;
		while(a[b]!='\0')
		{
			transmit(a[b]);
			b++;
		}
	}
}

