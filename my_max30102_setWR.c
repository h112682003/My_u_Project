#include "Nano100Series.h"              // Device header
#include "My_Project_MAX30102.h"

void set_write_2(uint8_t i,uint8_t j);
uint8_t set_read_1(uint8_t i,uint8_t j);

/*
Date:
202009060151
Note:
1.setting a lot(setWR creat write3)
2.waiting 0x06 0x04 when read(0x04) +1 do read6(0x07)
3.read6 red and ir(setWR creat read6)
*/


//--- setWrite_2 2-parameter
//set_write_2(REG ADDR, WriteData)
//REG ADDR: address 0x00~0xFF
//WriteData: 0x00~0xFF
//set_read_1(0xAE,0xFF)
void set_write_2(uint8_t i,uint8_t j){
	i2c_start();
	i2c_write(i);
	i2c_write(j);
}

//--- setRead_1 1-parameter
//set_read_1(REG ADDR, Ack)
//REG ADDR: address 0x00~0xFF
//Ack: 0(ACK) or 1(NACK)
//set_read_1(0xFF,1)
uint8_t set_read_1(uint8_t i,uint8_t j){
	int r;
	i2c_start();
	i2c_write(i);
	r = i2c_read(j);
	i2c_stop();
	return r;
}
