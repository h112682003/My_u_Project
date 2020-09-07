#include "Nano100Series.h"              // Device header
#include "My_Project_MAX30102.h"
#include "My_MAX30102.h"

void set_write_2(uint8_t i,uint8_t j);
uint8_t set_read_1(uint8_t i,uint8_t j);
void set_write_3(uint8_t i,uint8_t j,uint8_t k);

/*
Date:202009080132
Note:
1.need to add comments
*/


//--- setWrite_2 2-parameter
//set_write_2(REG ADDR, WriteData)
//REG ADDR: address 0x00~0xFF
//WriteData: 0x00~0xFF
//setWrite_2(0xAE,0xFF)
void set_write_2(uint8_t i,uint8_t j){
	i2c_start();
	i2c_write(i);
	i2c_write(j);
}

//--- setWrite_3 3-parameter
//set_write_3(REG ADDR, WriteData1, WriteData2)
//REG ADDR: address 0x00~0xFF
//WriteData: 0x00~0xFF
//setWrite_3(0xAE,0x09,0x43)
void set_write_3(uint8_t i,uint8_t j,uint8_t k){
	i2c_start();
	i2c_write(i);
	i2c_write(j);
	i2c_write(k);
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



