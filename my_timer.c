#include "Nano100Series.h"
#include "stdio.h"

void init_timer0(void);
void TMR0_IRQHandler(void);


void TMR0_IRQHandler(void){
	//printf("----ONESECOND----\n");
	TIMER_ClearIntFlag( TIMER0 );
}

void init_timer0(void){
	SYS_UnlockReg();
	//--- CLK
	CLK_EnableModuleClock(TMR0_MODULE);
	CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
	SYS_LockReg();
	//--- OPEN
	TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1);
	//--- Start
	TIMER_Start(TIMER0);
	//--- NVIC
  TIMER_EnableInt(TIMER0);
  NVIC_EnableIRQ(TMR0_IRQn);
}
