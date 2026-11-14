#include <reg51.h>

#define segment P0
#define sw P2
#define led P1

sbit r1 = P2^7;
sbit r2 = P2^6;
sbit r3 = P2^5;
sbit r4 = P2^4;

sbit c1 = P2^3;
sbit c2 = P2^2;
sbit c3 = P2^1;
sbit c4 = P2^0;

int keypad_scan();

void delay()
{
    int i, j;
    for(i = 0; i < 75; i++)
    {
			for(j = 0; j < 50; j++)
			{
			}
		}
}

int mode1_alt()
{
	int i;
	
	/*while(mode==1)
	{
		led = 0x01;
		delay();
		if(switch_mode())
		{
			return;
		}
		led = 0x02;
		delay();
		if(switch_mode())
		{
			return;
		}
		led = 0x04;
		delay();
		if(switch_mode())
		{
			return;
		}
		led = 0x08;
		delay();
		if(switch_mode())
		{
			return;
		}
	}*/
	int a;
	while(keypad_scan() != 0);
		while(1)
		{
			for(i=0;i<4;i++)
			{
				led = (2<<i);
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
			}
		}
	
}

int mode2_bincount()
{
	int a;
	unsigned char count = 0;
	while(keypad_scan() != 0);
	
	while(1)
	{
		if(count>15)
		{
			count = 0;
		}
		led = (count<<1);
		count++;
		delay();
		a = keypad_scan();
		if(a!=0)
		{
			
					return a;
		}
		delay();
		a = keypad_scan();
		if(a!=0)
		{
			
			return a;
		}
		delay();
		a = keypad_scan();
		if(a!=0)
		{
			
			return a;
		}
		delay();
		a = keypad_scan();
		if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
	}
}

int mode3_altpattern()
{
	int a;
	unsigned char pattern = 0x14;
	while(keypad_scan() != 0);
	/*unsigned char pattern = 0xF5;
	while(mode==3)
	{
		led = pattern;
		pattern = pattern ^ 0x0F;
		delay();
		if(switch_mode())
		{
			return;
		}
	}*/
	
	/*while(mode==3)
	{
		led = 0x0A;
		delay();
		if(switch_mode())
		{
			return;
		}
		led = 0x05;
		delay();
		if(switch_mode())
		{
			return;
		}
	}*/
	
	while(1)
	{
		led = pattern;
		pattern = pattern^(0xFF);
		delay();
		a = keypad_scan();
		if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
	}
}

int mode4_altpattern()
{
	int a;
	unsigned char pattern = 0x1F;
	while(keypad_scan() != 0);
	/*while(mode==4)
	{
		led = 0x0F;
		delay();
		if(switch_mode())
		{
			return;
		}
		led = 0x00;
		delay();
		if(switch_mode())
		{
			return;
		}
	}*/
	
	while(1)
	{
		led = pattern;
		pattern = pattern^ (0xFF);
		delay();
		a = keypad_scan();
		if(a!=0)
				{
					
					return a;
				}
				delay();
				a = keypad_scan();
				if(a!=0)
				{
					
					return a;
				}
	}
}

void display_digit(unsigned char value)
{
    if(value == '0')
        segment = ~(0x40);
    else if(value == '1')
        segment = ~(0x4F);
    else if(value == '2')
        segment = ~(0x24);
    else if(value == '3')
        segment = ~(0x30);
		else if(value == '4')
			segment = ~(0x19);
		else if(value == '5')
			segment = ~(0x12);
		else if(value == '6')
			segment = ~(0x02);
		else if(value == '7')
			segment = ~(0x78);
		else if(value == '8')
			segment = ~(0x00);
		else if(value == '9')
			segment = ~(0x10);
		else if(value == 'A')
			segment = ~(0x08);
		else if(value == 'B')
			segment = ~(0x03);
		else if(value == 'C')
			segment = ~(0x46);
		else if(value == 'D')
			segment = ~(0x21);
		else if(value == 'E')
			segment = ~(0x06);
		else if(value == 'F')
			segment = ~(0x0E);
}

void reset()
{
	segment = 0x00;
	led = 0x00;
}

int keypad_scan()
{
    int row = 0;

    sw = 0xF0;        

    if(r1 == 0)
        row = 1;
    else if(r2 == 0)
        row = 2;
    else if(r3 == 0)
        row = 3;
    else if(r4 == 0)
        row = 4;
    else
        return 0;

    delay();

    sw = 0x0F;          
	
    if(row == 1)
    {
        if(c1 == 0)
        {
					display_digit('0');
					led = 0x02;
					return 1;
					
				}
        else if(c2 == 0)
        {
					display_digit('1');
					led = 0x04;
					return 2;
				}
        else if(c3 == 0)
        {
					display_digit('2');
					led = 0x08;
					return 3;
				}
        else if(c4 == 0)
        {
					display_digit('3');
					led = 0x10;
					return 4;
				}
    }
	else if(row==2)
	{
		if(c1 == 0)
    {
			display_digit('4');
			return 5;
		}
        else if(c2 == 0)
				{
					display_digit('5');
					return 6;
				}
            
        else if(c3 == 0)
        {
					display_digit('6');
					return 7;
				}
        else if(c4 == 0)
				{
					display_digit('7');
					return 8;
				}
            
	}
	else if(row==3)
	{
				if(c1 == 0)
        {
					display_digit('8');
					led = 0x00;
					return 9;
				}
        else if(c2 == 0)
        {
					display_digit('9');
					return 10;
				}
        else if(c3 == 0)
				{
					led = 0x00;
					display_digit('A');
					return 11;
				}
            
        else if(c4 == 0)
				{
					led = 0x00;
					display_digit('B');
					return 12;
				}
	}
	else if(row==4)
	{
		led = 0x00;
				if(c1 == 0)
				{
					display_digit('C');
					return 13;
				}
        else if(c2 == 0)
				{
					display_digit('D');
					return 14;
				}
        else if(c3 == 0)
				{
					display_digit('E');
					return 15;
				}
        else if(c4 == 0)
				{
					display_digit('F');
					return 16;
				}
	}
return 0;
}

void main()
{
	int b;
    reset();
	b = 0;

    while(1)
    {
        if(b==0)
				{
					b = keypad_scan();
				}
			switch(b)
			{
				case 5: b = mode1_alt();
				break;
				
				case 6: b = mode2_bincount();
				break;
				
				case 7: b = mode3_altpattern();
				break;
				
				case 8: b = mode4_altpattern();
				break;
				
				default: b = 0;
			}
    }
}