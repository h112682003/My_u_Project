#include "Nano100Series.h" 

void SysTick_Handler(void);
void delay_ms(uint32_t ms);
void init_systick(void);
uint8_t get_systick(void);

//--------------- Systick ------------------
uint32_t tick=0;
void SysTick_Handler(void){
    tick++;
};
 
void delay_ms(uint32_t ms){
    ms += tick;
    while( ms != tick){}
}

uint8_t get_systick(void){
    return tick;
}
 
void init_systick(void){
    SysTick_Config( SystemCoreClock /1000);
}
