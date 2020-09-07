#include "Nano100Series.h" 

void init_HCLK_HXT_32M(void);
void init_UART1(uint32_t UART_TIME);
void UART1_IRQHandler(void);
	
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

void init_UART1(uint32_t UART_TIME)
{
	SYS_UnlockReg();
	//--- CLK
	CLK_EnableModuleClock(UART1_MODULE);
	CLK_SetModuleClock(UART1_MODULE,CLK_CLKSEL1_UART_S_HXT,CLK_UART_CLK_DIVIDER(1));
	//--- PIN
	SYS->PB_L_MFP &= ~( SYS_PB_L_MFP_PB0_MFP_Msk | SYS_PB_L_MFP_PB1_MFP_Msk);
	SYS->PB_L_MFP |= (SYS_PB_L_MFP_PB4_MFP_UART1_RX | SYS_PB_L_MFP_PB5_MFP_UART1_TX );//PB4<-RX;PB5->TX;
	SYS_LockReg();
	//--- OPEN
	UART_Open(UART1, UART_TIME);
	//--- NVIC
	UART_ENABLE_INT(UART1, UART_IER_RDA_IE_Msk);
	NVIC_EnableIRQ(UART1_IRQn);
}

//--- ISR
void UART1_IRQHandler(void)
{
		uint8_t u8InChar;
		u8InChar = UART_READ(UART1); /* Rx trigger level is 1 byte*/
		UART_WRITE(UART1,u8InChar);
}
