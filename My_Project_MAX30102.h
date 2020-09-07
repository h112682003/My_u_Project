#ifndef __My_Project_MAX30102__
#define __My_Project_MAX30102__

#include <stdint.h>

//kd1602.c
void lcd_print(uint8_t pos, char *s);
void lcd_init(void);

//my_i2c.c
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t d);
uint8_t i2c_read(uint8_t ack);

//my_systick.c
void SysTick_Handler(void);
void delay_ms(uint32_t ms);
void init_systick(void);

//my_uart.c
void init_HCLK_HXT_32M(void);
void init_UART0(uint32_t UART_TIME);
void UART0_IRQHandler(void);

//my_bh1750.c
uint8_t bh1750_test(void);
int16_t bh1750(void);

//my_timer.c
void init_timer0(void);
void TMR0_IRQHandler(void);

#endif
