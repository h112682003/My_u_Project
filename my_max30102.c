#include "Nano100Series.h"              // Device header
#include "My_Project_MAX30102.h"

//--- Header
uint8_t hr_spo2_id1(void);
uint8_t hr_spo2_id2(void);
//---

/*
Date:
202009060151
Note:
1.setting a lot(setWR creat write3)
2.waiting 0x06 0x04 when read(0x04) +1 do read6(0x07)
3.read6 red and ir(setWR creat read6)
*/


//--- Part ID REG ADDR is 0xFF
uint8_t hr_spo2_id1(void){
	int r;
	set_write_2(0xAE,0xFF);
	r = set_read_1(0xAF,1);
	return r;
}

//--- Revision ID REG ADDR is 0xFE
uint8_t hr_spo2_id2(void){
	int r;
	set_write_2(0xAE,0xFE);
	r = set_read_1(0xAF,1);
	return r;
}
