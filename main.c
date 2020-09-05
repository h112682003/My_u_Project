#include "Nano100Series.h"              // Device header
#include "stdio.h"
#include "My_Project_MAX30102.h"


int main(void){

	//Parameters setting
	int r, s=0, m=0, h=0;
	char bf[32];
	//Initialize
	lcd_init();// LCD
	init_systick();// Systick
	init_timer0();
	GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);
	PA12 = 0;
	
	/*
	while(1){
		while(TIMER_GetIntFlag(TIMER0)==0){
			TIMER_ClearIntFlag(TIMER0);
			TIMER_Delay(TIMER0,1000000);
			PA12 ^= 1;
		}//--- while Timer0
	}//--- while 1
	*/
	
	//Loop
	while(1){
		r=bh1750();
		if(s==60){s=0;m+=1;}
		if(m==60){m=0;h+=1;}
		if(r>0){
				sprintf(bf, "RT:%4dH%3dM%3dS", h, m, s);
				lcd_print(0x00,bf);
				sprintf(bf, "LUX: %d", r);
				lcd_print(0x40,bf);
		}
		s++;
		PA12 ^= 1;
		TIMER_Delay(TIMER0,410000);
		PA12 ^= 1;
		TIMER_Delay(TIMER0,410000);
		//delay_ms(820);//because bh1750() has a 180ms delay
		
	}//--- while
}//--- main
