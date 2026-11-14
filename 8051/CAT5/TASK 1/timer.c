#include <reg51.h>
sbit led = P3^1;

void main(){
      int cnt =0;
      TMOD = 0x01;
			TH0 = 0xFC;
      TL0 = 0x66;
      TR0 = 1;
      led = 0;
	
      while(1)
				{
      if( TF0==1)
				{
          cnt++;
					TF0 = 0;
					TMOD = 0x01;
					TH0 = 0xFC;
					TL0 = 0x66;
					TR0 = 1;
							
					
				}
					if(cnt == 600)
					{
						led = ~led;
						cnt = 0;
					}
						
			}
}

