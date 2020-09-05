#include "Nano100Series.h"              // Device header
#include "stdio.h"

#define Time1	54
#define	RS	PC15
#define RW	PC14
#define E		PB15

void lcd_init(void);
void lcd_print(uint8_t pos, char *s);

void delay(int n){
	int i;
	for(i=0;i<n;i++){}
}


void lcd_cmd(uint8_t c){
	RW=0; RS=0;
	PD->DOUT = c;
	E=1;	delay(0);	E=0;
	delay(Time1);
}

void lcd_put(uint8_t c){
	RW=0; RS=1;
	PD->DOUT = c;
	E=1;	delay(0);	E=0;
	delay(Time1);
}

void lcd_print(uint8_t pos, char *s){
	lcd_cmd(0x80 |pos);
	while(*s){
		lcd_put(*s++);
	}
}

void lcd_init(void){
	//--- LCD
	GPIO_SetMode(PB, BIT15, GPIO_PMD_OUTPUT); //Set GPIOB_PMD15 to OUTPUT mode
	GPIO_SetMode(PC, BIT14+BIT15, GPIO_PMD_OUTPUT); //Set GPIOC_PMD14~15 to OUTPUT mode
	GPIO_SetMode(PD, BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT7+BIT7, GPIO_PMD_OUTPUT); //Set GPIOD_PMD0~7 to OUTPUT mode
	E = 0;
	//--- STEP1
	delay(1000000);
	//--- STEP2
	lcd_cmd(0x38);
	//--- STEP3
	lcd_cmd(0x0C); //lcd_cmd(0x0E)  E 有底線  C 沒有底線
	
	//--- STEP3.5
	lcd_cmd(0x80);
	//--- STEP4	
	lcd_cmd(0x06);
	//--- STEP5
	lcd_print(0x00, "                ");
	lcd_print(0x04, "                ");
}
