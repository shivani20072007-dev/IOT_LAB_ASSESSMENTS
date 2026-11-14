#include<reg51.h>
#define D P3
sbit D4 = P3^4;
sbit D5 = P3^5;
sbit D6 = P3^6;
sbit D7 = P3^7;
sbit E = P3^3;
sbit RS = P3^2;

void delay()
{
	int i,j;	
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
		{    
		}
	}
}

void lcd_init()
{
	RS = 0;
	//Clear display
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	//D = (0x01)|0xF0;
	E = 1;
	delay();
	E = 0;
	D4 = 1;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	//D = (0x01<<4)&0xF0;
	E = 1;
	delay();
	E = 0;
	
	
	//Function set
	D4 = 0;
	D5 = 1;
	D6 = 0;
	D7 = 0;
	//D = 0x28;
	E = 1;
	delay();
	E = 0;
	//D = (0x28<<4);
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 1;
	E = 1;
	delay();
	E = 0;
	
	//Display lines
	//D = 0x0F;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	//D = (0x0F<<4);
	D4 = 1;
	D5 = 1;
	D6 = 1;
	D7 = 1;
	E = 1;
	delay();
	E = 0;
	
	//Entry mode set
	//D = 0x06;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	//D = (0x06<<4);
	D4 = 0;
	D5 = 1;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
}

void main()
{
	lcd_init();
	RS = 1;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	
	E = 0;
	delay();
	
	RS = 1;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 1;
	D5 = 0;
	D6 = 0;
	D7 = 1;
	E = 1;
	E = 0;
	delay();
	
	RS = 1;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 0;
	D5 = 1;
	D6 = 1;
	D7 = 1;
	E = 1;
	E = 0;
	delay();
	/*while(1)
	{
		lcd_init();
	}*/
	
	RS = 0;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 1;
	E = 1;
	delay();
	E = 0;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 0;
	E = 1;
	E = 0;
	
	RS = 1;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 1;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	E = 0;
	delay();
	
	RS = 1;
	D4 = 1;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 1;
	D5 = 1;
	D6 = 0;
	D7 = 0;
	E = 1;
	E = 0;
	delay();
	
	RS = 1;
	D4 = 0;
	D5 = 0;
	D6 = 1;
	D7 = 0;
	E = 1;
	delay();
	E = 0;
	D4 = 0;
	D5 = 0;
	D6 = 0;
	D7 = 1;
	E = 1;
	E = 0;
	delay();
}