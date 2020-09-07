#include "Nano100Series.h"              // Device header
#include "stdio.h"
#include "My_Project_MAX30102.h"
#include "My_MAX30102.h"

int main(void){
	//Parameters setting
	int i, r, w, ava, ri, red, ired, sum_red, sum_ired, avg_red, avg_ired;
	int max, min;
	int max_red_x, max_red_y, min_red0_x, min_red0_y, min_red1_x, min_red1_y;
	int max_ired_x, max_ired_y, min_ired0_x, min_ired0_y, min_ired1_x, min_ired1_y;
	int Ared[32], Aired[32];
	short int state;
	volatile short int CP;
	int red_y, ired_y, x;
	float AC_red, DC_red, AC_ired, DC_ired;
	float R ,spo2;
	int stage = 10000;
	char bf[64];

	//Initialize
	lcd_init();// LCD
	init_systick();// Systick
	init_UART1(115200);
	hr_spo2_init();
	red=0; ired=0, ava=0, ri=0, sum_red=0, sum_ired=0, avg_red=0, avg_ired=0;
	max=0, min=3800000;
	max_red_x=0, max_red_y=0, min_red0_x=0, min_red0_y=0, min_red1_x=0, min_red1_y=0;
	max_ired_x=0, max_ired_y=0, min_ired0_x=0, min_ired0_y=0, min_ired1_x=0, min_ired1_y=0;
	red_y=0, ired_y=0, x=0;
	AC_red=0, DC_red=0, AC_ired=0, DC_ired=0;
	state = 0;
	R = 0;
	CP=50; //Change Point Finder
	
	//INIT LCD Print "Demo"
	lcd_print(0x00,"Demo  2020/09/28");
	GPIO_SetMode(PA, BIT12+BIT13, GPIO_PMD_OUTPUT);
	PA12 = 0;
	PA13 = 0;
	//Loop
	while(1){
		//Get MAX30102 R/W Pointer
		r=hr_spo2_wait_PR_ready();
		w=hr_spo2_wait_WR_ready();

		//Count ava for data availability
		ava = w-r;
		
		if(ava<0) ava+=32; //Wrap condition
		//以下可改成 if(ava==0){contimue;} 就可以少掉一層大括括弧
		//Get red and ired data
		if(ava>0){
			
			//hr_spo2_get_data(bf);
			i2c_start();
			i2c_write(0xAE);
			i2c_write(0x07);
			i2c_start();
			i2c_write(0xAF);
			red = 65536*i2c_read(0);
			red += 256*i2c_read(0);
			red += i2c_read(0);
			ired = 65535*i2c_read(0);
			ired += 256*i2c_read(0);
			ired += i2c_read(1);
			i2c_stop();
			
			Ared[ri] = red;	Aired[ri] = ired; ri++;
			
			#define AVGN 4
			if(AVGN){
			sum_red = 0;
			sum_ired = 0;
			for(i=0;i<AVGN;i++){
				sum_red += Ared[i];
				sum_ired += Aired[i];
			}
			avg_red = sum_red/AVGN;
			avg_ired = sum_ired/AVGN;
			if(ri>(AVGN-1))ri=0;
		}
			//Find SpO2
			
			x++;//red_x++, ired_x++;//axis-x counter for time
			red_y = avg_red;//axis-y counter for red
			ired_y = avg_ired;//axis-y counter for ired
			if(avg_red<stage && avg_ired<stage)state=0; //if no finger back state 0
			switch(state){
				
				case 0://No finger statement
					x=0; //axis-x counter set 0
					lcd_print(0x00,"Where is your   ");
					lcd_print(0x40,"finger ?");
					if(avg_red>stage && avg_ired>stage)state=1;
					break;
				
				case 1: //if up find max
					if(red_y>max){
						//red
						max_red_y = red_y; max_red_x = x;
						//ired
						max_ired_y = ired_y; max_ired_x = x;
						//reset max
						max = red_y;
					}else{ 
						if(max-red_y>CP){	min = max; state = 2; PA12=1;}
					}
					break;
				
				case 2://if down and new=0
					if(red_y<min){
						//red
						min_red1_y = red_y; min_red1_x = x;
						//ired
						min_ired1_y = ired_y; min_ired1_x = x;
						//reset min
						min = red_y;
					}else{
						if(red_y-min>CP){	max = min; state = 3; PA12=0;}
					}
					break;

				case 3: //if up find max
					if(red_y>max){
						//red
						max_red_y = red_y; max_red_x = x;
						//ired
						max_ired_y = ired_y; max_ired_x = x;
						//reset max
						max = red_y;
					}else{ 
						if(max-red_y>CP){	min = max; state = 4; PA12=1;}
					}
					break;	
					
				case 4://Push min1>>min0
						//red
						min_red0_x = min_red1_x; min_red0_y = min_red1_y;
						//ired
						min_ired0_x = min_ired1_x; min_ired0_y = min_ired1_y;
						state = 5;
					
				case 5://if down and new=0
					if(red_y<min){
						//red
						min_red1_y = red_y; min_red1_x = x;
						//ired
						min_ired1_y = ired_y; min_ired1_x = x;
						//reset min
						min = red_y;
					}else{
						if(red_y-min>CP){	max = min; state = 6; PA12=0;}
					}
					break;
					
				case 6://Print SpO2
					//DC&AC只用在這 宣告在這就好
					DC_red = ((min_red1_y-min_red0_y)/(min_red1_x-min_red0_x))*(max_red_x-min_red0_x)+ min_red0_y;
					AC_red = max_red_y - DC_red;
					DC_ired = ((min_ired1_y-min_ired0_y)/(min_ired1_x-min_ired0_x))*(max_ired_x-min_ired0_x)+ min_ired0_y;
					AC_ired = max_ired_y - DC_ired;
					R = (AC_red/DC_red)/(AC_ired/DC_ired);
					spo2 = 104-(2.61*R);
					state = 3;
					lcd_print(0x00,"Demo  2020/09/28");
					sprintf(bf, "SpO2: %3.2f%%      ",spo2);
					lcd_print(0x40,bf);
					break;
					
			}
		}
	}//--- while
}//--- main
