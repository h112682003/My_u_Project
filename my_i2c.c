#include "Nano100Series.h" 
#pragma anon_unions

void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t d);
uint8_t i2c_read(uint8_t ack);

//  SCL -- PF5
//  SDA -- PF4
// ADDR -- PB8
 
#define     SCL     PF5
#define     SDA     PF4

typedef union{
    unsigned char v;
    struct{
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
}TPU8;
 
//--- Delay
static void delay(int n){
    int i;
    for(i=0;i < n;i++){}
}

//--- Start
void i2c_start(void){
		//--- PF4~5 open drain
		GPIO_SetMode(PF, (BIT4+BIT5), GPIO_PMD_OPEN_DRAIN);

    PF->PUEN |=  BIT4+BIT5;
 
    if(SDA==0){
        SCL=0; delay(0);
    }
    SDA=1; delay(0);
    SCL=1; delay(0);
    SDA=0; delay(0);
    SCL=0; delay(0);
}

//--- Stop
void i2c_stop(void){
    SDA=0;  delay(0);
    SCL=1;  delay(0);
    SDA=1;  delay(0);
}

//--- Write
uint8_t i2c_write(uint8_t d){
    TPU8 r;
    r.v = d;
    SDA=r.b7; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b6; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b5; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b4; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b3; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b2; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b1; delay(0); SCL=1; delay(0); SCL=0;
    SDA=r.b0; delay(0); SCL=1; delay(0); SCL=0;
    //--- ACK
    SDA=1; delay(0); SCL=1; delay(0); r.v=SDA; SCL=0;
     
    return r.v;
}

//--- Read
uint8_t i2c_read(uint8_t ack){
    TPU8 r;
    SDA=1;
    delay(0); SCL=1; delay(0); r.b7 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b6 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b5 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b4 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b3 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b2 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b1 = SDA; SCL=0;
    delay(0); SCL=1; delay(0); r.b0 = SDA; SCL=0;
    //--- ACK
    SDA = ack; delay(0); SCL=1; delay(0); SCL=0;
     
    SDA=1;
    return r.v;
}
