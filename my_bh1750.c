#include "Nano100Series.h"     
#include "My_Project_MAX30102.h"

uint8_t bh1750_test(void);
int16_t bh1750(void);

//--- BH1750 test
uint8_t bh1750_test(void){
	int i, r;
	for(i=2; i<254; i+=2){
		i2c_start();
		r = i2c_write(i);
		if(r==0){break;}
	}
	i2c_stop();
	return i;

}//--- bh1750_test

int16_t bh1750(void){
	
	int ans=0;
	i2c_start();
	i2c_write(0x46+0);
	i2c_write(0x10);
	i2c_stop();
	
	delay_ms(180);
	
	i2c_start();
	i2c_write(0x46+1);
	ans = i2c_read(0)*256;
	ans += i2c_read(1);
	i2c_stop();

	return ans;

}//--- bh1750
