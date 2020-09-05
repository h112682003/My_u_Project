#include "Nano100Series.h" 

void init_HCLK_HXT_32M(void);
void init_UART0(uint32_t UART_TIME);
void UART0_IRQHandler(void);
	
void init_HCLK_HXT_32M(void)
{
	SYS_UnlockReg();
	//--- CLK
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT,CLK_HCLK_CLK_DIVIDER(1));
	CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk | CLK_PWRCTL_LXT_EN_Msk | CLK_PWRCTL_HIRC_EN_Msk);
	CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk | CLK_CLKSTATUS_LXT_STB_Msk | CLK_CLKSTATUS_HIRC_STB_Msk);
	CLK_SetCoreClock(32000000);
	SYS_LockReg();
}

void init_UART0(uint32_t UART_TIME)
{
	SYS_UnlockReg();
	//--- CLK
	CLK_EnableModuleClock(UART0_MODULE);
	CLK_SetModuleClock(UART0_MODULE,CLK_CLKSEL1_UART_S_HXT,CLK_UART_CLK_DIVIDER(1));
	//--- PIN
	SYS->PB_L_MFP &= ~( SYS_PB_L_MFP_PB0_MFP_Msk | SYS_PB_L_MFP_PB1_MFP_Msk);
	SYS->PB_L_MFP |= (SYS_PB_L_MFP_PB0_MFP_UART0_RX | SYS_PB_L_MFP_PB1_MFP_UART0_TX );//PB0->RX;PB1->TX;
	SYS_LockReg();
	//--- OPEN
	UART_Open(UART0, UART_TIME);
	//--- NVIC
	UART_ENABLE_INT(UART0, UART_IER_RDA_IE_Msk);
	NVIC_EnableIRQ(UART0_IRQn);
}

//--- ISR
void UART0_IRQHandler(void)
{
		uint8_t u8InChar;
		u8InChar = UART_READ(UART0); /* Rx trigger level is 1 byte*/
		UART_WRITE(UART0,u8InChar);
}
