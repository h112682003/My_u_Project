#include "Nano100Series.h"              // Device header
#include "My_Project_MAX30102.h"
#include "My_MAX30102.h"

//--- Header
uint8_t hr_spo2_part_id(void);
uint8_t hr_spo2_revision_id(void);
void hr_spo2_init(void);
uint8_t hr_spo2_wait_PR_ready(void);
uint8_t hr_spo2_wait_WR_ready(void);
void hr_spo2_get_data(char *buf);
//---

//--- Structure for FIFO Read

//---

/*

Date:202009080132
Note:
1.need to add comments

*/


//--- Part ID (REG ADDR is 0xFF)
uint8_t hr_spo2_part_id(void){
	int r;
	set_write_2(0xAE,0xFF);
	r = set_read_1(0xAF,1);
	return r;
}

//--- Revision ID (REG ADDR is 0xFE)
uint8_t hr_spo2_revision_id(void){
	int r;
	set_write_2(0xAE,0xFE);
	r = set_read_1(0xAF,1);
	return r;
}

//--- RESET by Mode Configuration (REG ADDR is 0x09)
//RSEST 1
void hr_spo2_reset(void){
	int r, w;
	uint8_t REG_ADDR = 0x09;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x40;//RSEST 1
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=0x00);
}

//--- SMP_AVG by FIFO Configuration (REG ADDR is 0x08)
//SMP_AVE[2:0]=#of samples avg. 000=1, 001=2, 010=4, 011=8, 100=16, 101=32, 110=32, 111=32
void hr_spo2_avg(void){
	int r, w;
	uint8_t REG_ADDR = 0x08;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x40;//setting 4 samples avg.
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- FIFO_ROLL_OVER_EN by FIFO Configuration (REG ADDR is 0x08)
//FIFO_ROLL_OVER_EN 1=Enable, 0=Disable
void hr_spo2_fifo_roll(void){
	int r, w;
	uint8_t REG_ADDR = 0x08;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x10;//setting Enable
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- Mode Control by Mode Configuration (REG ADDR is 0x09)
//MODE[2:0]=ACTIVE LED CHANNELS  010=Heart Rate(only red), 011=SpO2(red+IR), 111=Multi-LED(red+IR)
void hr_spo2_mode_set(void){
	int r, w;
	uint8_t REG_ADDR = 0x09;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x03;//setting Mode as SpO2
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- SpO2_ADC Range Control by SpO2 Configuration (REG ADDR is 0x0A)
//SpO2_ADC_RGE[1:0]=SIZE   00=2048(nA), 01=4096(aA), 10=8192(nA), 11=16384(nA)
void hr_spo2_adc_rge(void){
	int r, w;
	uint8_t REG_ADDR = 0x0A;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x20;//setting 01=4096(aA)
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- SpO2_Rate Control by SpO2 Configuration (REG ADDR is 0x0A)
//SpO2_SR[2:0]=#samples/sec   000=50, 001=100, 010=200, 011=400, 100=800, 101=1000, 110=1600, 111=3200
void hr_spo2_sr(void){
	int r, w;
	uint8_t REG_ADDR = 0x0A;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x04;//setting 010=200samples/sec
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- SpO2_Pulse Width Control by SpO2 Configuration (REG ADDR is 0x0A)
//SpO2_SR[2:0]=ADC resolution(bits)   00=15, 01=16, 10=17, 11=18
void hr_spo2_led_pw(void){
	int r, w;
	uint8_t REG_ADDR = 0x0A;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x03;//setting 11=18(bits)
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}

//--- LED Pluse Amplitude by LED Pluse Amplitude (REG ADDR is 0x0C, 0x0D)
//SpO2_SR[2:0]=ADC resolution(bits) see Table 8.LED Current Control
void hr_spo2_led_pa(void){
	uint8_t data = 0x3C;// 0x3C = 60, Current = 60*0.2=1.2mA
	set_write_3(0xAE,0x0C,data);//LED1_PA
	set_write_3(0xAE,0x0D,data);//LED2_PA
	set_write_3(0xAE,0x0E,data);//RESERVED
	set_write_3(0xAE,0x10,data);//RESERVED
}

//--- FIFO data structure by SpO2 Multi-LED Mode Control Resgisters (REG ADDR for SLOT1&2 is 0x11, SLOT3&4 is 0x12)
//SLOT1
void hr_spo2_led_fifo_slot1(void){
	int r, w;
	uint8_t REG_ADDR = 0x11;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x01;//setting first data is red
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}	
//SLOT2
void hr_spo2_led_fifo_slot2(void){
	int r, w;
	uint8_t REG_ADDR = 0x11;
	do{
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	w = r|0x20;//setting second data is IR
	set_write_3(0xAE,REG_ADDR,w);
	set_write_2(0xAE,REG_ADDR);
	r = set_read_1(0xAF,1);
	}while(r!=w);
}	

//--- Clear FIFO W/R Pointer & Overflow Counter (REG ADDR for W/R is 0x04/0x06, Overflow is 0x05)
void hr_spo2_init_ptr(void){
	set_write_3(0xAE,0x04,0x00);//FIFO_WR_PTR
	set_write_3(0xAE,0x05,0x00);//OVF_COUNTER
	set_write_3(0xAE,0x06,0x00);//FIFO_RD_PTR
}

//init MAX30102
void hr_spo2_init(void){
	hr_spo2_reset();
	hr_spo2_avg();
	hr_spo2_fifo_roll();
	hr_spo2_mode_set();
	hr_spo2_adc_rge();
	hr_spo2_sr();
	hr_spo2_led_pw();
	hr_spo2_led_pa();
	hr_spo2_led_fifo_slot1();
	hr_spo2_led_fifo_slot2();
	hr_spo2_init_ptr();
}
//Wait the data ready by read the RP_PTR(0x06)
uint8_t hr_spo2_wait_PR_ready(void){
	int r;
	i2c_start();
	i2c_write(0xAE);
	i2c_write(0x06);
	i2c_start();
	i2c_write(0xAF);
	r = i2c_read(1);
	i2c_stop();
	return r;
}

//Wait the data ready by read the WR_PTR(0x04)
uint8_t hr_spo2_wait_WR_ready(void){
	int r;
	i2c_start();
	i2c_write(0xAE);
	i2c_write(0x04);
	i2c_start();
	i2c_write(0xAF);
	r = i2c_read(1);
	i2c_stop();
	return r;
}

void hr_spo2_get_data(char *bf){
	i2c_start();
	i2c_write(0xAE);
	i2c_write(0x07);
	i2c_start();
	i2c_write(0xAF);
	bf[0] = i2c_read(0);
	bf[1] = i2c_read(0);
	bf[2] = i2c_read(0);
	bf[3] = i2c_read(0);
	bf[4] = i2c_read(0);
	bf[5] = i2c_read(1);
	i2c_stop();
}









