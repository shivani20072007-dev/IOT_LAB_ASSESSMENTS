#include<reg51.h>
#define switch P2
sbit D4 = P3^4;
sbit D5 = P3^5;
sbit D6 = P3^6;
sbit D7 = P3^7;
sbit E = P3^3;
sbit RS = P3^2;

sbit sw1 = P2^3;
sbit sw = P2^7;
sbit sw2 = P2^2;
sbit sw3 = P2^1;
sbit sw4 = P2^0;

unsigned char pos = 0x80;

void delay()
{
	int i,j;
	for(i=0;i<100;i++)
	{
		for(j=0;i<100;j++)
		{
		}
	}
}

void clear_display()
{
	RS = 0;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	
	
	D4 = 1;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
}

void right()
{
	if(pos<0x8F)
	{
		pos = pos + 0x01;
	}
	else
	{
		pos = 0xC0;
	}
	clear_display();
	display_char('A');
}

void left()
{
	if(pos>0xC0)
	{
		pos = 0x8F;
	}
	else
	{
		pos = pos - 0x01;
	}
	clear_display();
	display_char('A');
}

void up()
{
	if((pos>=0xC0)&&(pos<=0xCF))
	{
		pos = pos - 0x40;
	}
	clear_display();
	display_char('A');
}

void down()
{
	if(~((pos>=0xC0)&&(pos<=0xCF)))
	{
		pos = pos + 0x40;
	}
	clear_display();
	display_char('A');
}

void button()
{
	if(sw1==0)
	{
		right();
	}
	if(sw2==0)
	{
		left();
	}
	if(sw3==0)
	{
		down();
	}
	if(sw4==0)
	{
		up();
	}
}