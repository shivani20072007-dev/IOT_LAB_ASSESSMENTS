#include<reg51.h>
#define led P1
sbit row1 = P2^7;
sbit col1 = P2^3;

unsigned char mode = 0;

void delay()
{
	int i,j;
	for(i=0;i<200;i++)
	{
		for(j = 0;j<200;j++)
		{
		}
	}
}

bit switch_mode()
{
	P2 = 0x7F;
	if(col1==0)
	{
		delay();
		if(col1==0)
		{
			while(col1==0)
			{
			}
			mode++;
			if(mode>4)
			{
				mode = 1;
			}
			row1 = 1;
			return 1;
		}
	}
	row1 = 1;
	return 0;
}

void mode1_alt()
{
	int i;
	led = 0x0F;
	while(mode==1)
	{
		for(i=0;i<4;i++)
		{
			led = ~(1<<i);
			delay();
			if(switch_mode())
			{
				return;
			}
		}
	}
	led = 0xFF;
}

void mode2_bincount()
{
	unsigned char count = 0;
	while(mode==2)
	{
		if(count>15)
		{
			count = 0;
		}
		led = ~count;
		count++;
		delay();
		if(switch_mode())
		{
			return;
		}
	}
}

void mode3_altpattern()
{
	unsigned char pattern = 0xF5;
	while(mode==3)
	{
		led = pattern;
		pattern = pattern ^ 0x0F;
		delay();
		if(switch_mode())
		{
			return;
		}
	}
}

void mode4_altpattern()
{
	unsigned char pattern = 0x00;
	while(mode==4)
	{
		led = pattern;
		pattern = pattern ^ 0x0F;
		delay();
		if(switch_mode())
		{
			return;
		}
	}
}

void main()
{
	led = 0xFF;
	P2 = 0xFF;
	while(1)
	{
		switch_mode();
		switch(mode)
		{
			case 0: led = 0xFF;
			break;
			
			case 1: mode1_alt();
			break;
			
			case 2: mode2_bincount();
			break;
			
			case 3: mode3_altpattern();
			break;
			
			case 4: mode4_altpattern();
			break;
		}
	}
}