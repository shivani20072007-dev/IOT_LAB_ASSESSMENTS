#include<reg51.h>
sbit led = P3^1;
int count = 1;
int direction = 1;

void controller(int a)
{
	/*if(a>10)
	{
		a = 11 - (a%10);
	}*/
	if(a==1)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xAB)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	else if(a==2)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xB5)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	else if(a==3)
	{
		int rand = 0;
		while(rand<10000)
		{
				while(TL0<=0xBE)
				{
					led = 1;
				}
				while(TF0==0)
				{
					led = 0;
				}
				TF0 = 0;
				TH0 = 0xFF;
				TL0 = 0xA3;
				rand++;
				TR0 = 1;
		}
		
	}
	else if(a==4)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xC7)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==5)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xD0)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==6)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xD9)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==7)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xE2)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==8)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xEE)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==9)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TL0<=0xF5)
			{
				led = 1;
			}
			while(TF0==0)
			{
				led = 0;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
	
	else if(a==10)
	{
		int rand = 0;
		while(rand<10000)
		{
			while(TF0==0)
			{
				led = 1;
			}
			TF0 = 0;
			TH0 = 0xFF;
			TL0 = 0xA3;
			rand++;
			TR0 = 1;
		}
	}
}

void main()
{
	TMOD = 0x01;
	TH0 = 0xFF;
	TL0 = 0xA3;
	TR0 = 1;
	led = 0;
	while(1)
	{
		/*if(count<=10)
		{
			controller(count);
			count++;
		}
		else
		{
			controller(count);
			count--;
		}
		if(count>20)
		{
			count = 1;
		}
		
		TH0 = 0xFF;
		TL0 = 0xA3;
		TR0 = 1;*/
		controller(count);

    if(count == 10)
    {
        direction = -1;
    }
    else if(count == 1)
    {
        direction = 1;
    }

    count = count + direction;

    TH0 = 0xFF;
    TL0 = 0xA3;
    TR0 = 1;
		
	}
}