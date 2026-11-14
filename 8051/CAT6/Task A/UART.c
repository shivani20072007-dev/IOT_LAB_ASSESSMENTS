#include <reg51.h>
int counter = 0;
char func(char s[])
{
    if(counter >= 5)
        counter = 0;
    return s[counter++];
}
void main()
{
    char s[] = "hello";
    TMOD = 0x20;
    TH1 = 0xFD;
    TL1 = 0xFD;
    SCON = 0x50;
    TR1 = 1;
            while(1)
    {
        SBUF = func(s);
        while(TI == 0);
        TI = 0;
    }
}

/*#include<reg51.h>

void first()
{
	SCON = 0x50;
	TMOD = 0x20;
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
		char a[] = "SDFSHFIFSKF\n";
		int b = 0;
		while(a[b]!='\0')
		{
			transmit(a[b]);
			b++;
		}
	}
}*/

