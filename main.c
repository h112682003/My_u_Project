#include "Nano100Series.h"              // Device header
#include "stdio.h"
#include "My_Project_MAX30102.h"
#include "My_MAX30102.h"


int main(void){
	//Parameters setting
	int i, r, w;
	int red=0, ired=0;
	char bf[6];

	//Initialize
	lcd_init();// LCD
	init_systick();// Systick
	init_timer0();
	hr_spo2_init();
	i=0; red=0; ired=0;
	lcd_print(0x00,"Hello JOJO");
	lcd_print(0x40,"DATA: N/A");
	GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);
	PA12 = 0;
	
	//Loop
	while(1){
		r=hr_spo2_wait_PR_ready();
		w=hr_spo2_wait_WR_ready();
		TIMER_Delay(TIMER0,1000);
		if(w-r>0){
			//hr_spo2_get_data(bf);
			i2c_start();
			i2c_write(0xAE);
			i2c_write(0x07);
			i2c_start();
			i2c_write(0xAF);
			red += 65536*i2c_read(0);
			red += 256*i2c_read(0);
			red += i2c_read(0);
			ired += 65535*i2c_read(0);
			ired += 256*i2c_read(0);
			ired += i2c_read(1);
			i2c_stop();
			TIMER_Delay(TIMER0,1000);
			i++;
			if(i==10){
				sprintf(bf, "Red:%12d", red/10);
				lcd_print(0x00,bf);
				sprintf(bf, "IR:%13d", ired/10);
				lcd_print(0x40,bf);
				i=0; red=0; ired=0;
				PA12 ^= 1;
			}
		}
		if(r-w>0){
			set_write_3(0xAE,0x04,0x00);//FIFO_WR_PTR
			set_write_3(0xAE,0x05,0x00);//OVF_COUNTER
			set_write_3(0xAE,0x06,0x00);//FIFO_RD_PTR
		}
	}//--- while
}//--- main
