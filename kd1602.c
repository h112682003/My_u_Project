#include "Nano100Series.h"              // Device header
void lcd_print(uint8_t pos, char *s);
void lcd_init(void);
 
//------------------- LCD ------------------
#define RS  PC15
#define RW  PC14
#define E   PB15
 
static void delay(uint32_t t){
    int i;
    for(i=0; i < (t*5); i++){}
}
//---
void lcd_cmd(uint8_t c){
    RS=0; RW=0;
    PD->DOUT = c;
    delay( 10 ); E=1; delay( 10 ); E=0;
    delay( 100 );
}
 
//---
void lcd_put(uint8_t c){
    RS=1; RW=0;
    PD->DOUT = c;
    delay( 10 ); E=1; delay( 10 ); E=0;
    delay( 100 );
}
#define LCD_ADDR(x)    lcd_cmd( 0x80 | x )
void lcd_print(uint8_t pos, char *s){
    //--- ????
    lcd_cmd(0x80 | pos);
    //--- ???
    while(*s){ lcd_put(*s++); }
}
 
//---
void lcd_init(void){

    //--- GPIO
    PB->PMD |= 0x40000000;
    PC->PMD |= 0x50000000;
    PD->PMD |= 0x00005555;
    PD->DMASK |= 0x0000FF00;
    RS=0; RW=0; E=0;
    //--- example
    delay(500000);
    lcd_cmd(0x38);      //step 2
    lcd_cmd(0x0C);      //step 3
    lcd_cmd(0x06);      //step 4
    lcd_cmd(0x80);      //step 4.5
    //---
    lcd_print(0x00, "                ");
    lcd_print(0x40, "                ");
}


