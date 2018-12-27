#include "gct301s.h"

UART_TypeDef *uart = UART0;

volatile uint32_t SYSTICK = 0;

void SysTick_Handler(void)
{
    (void)SysTick->CTRL;        // read to clear flag
    SYSTICK++;
}

void delay_ms(uint32_t d)
{
    uint32_t end_tick = SYSTICK +  d;

    while (SYSTICK < end_tick) {
        __NOP();
    }
}

void GPIO_Init(void)
{
    // TX0 & RX0
    GPIOC->ALTFUNCSET = 0x3;
}

void DPLL_Init(void)
{
    // turn on DPLL
    SYSCTRL->DPLL_CTL = 0x1;
    while ((SYSCTRL->SYS_CLK_STA & 0x1) == 0);
}

void SER_Init(void)
{
    // uart frame settings
    uart->CLKEN = 0;
    while ((uart->CLKEN & 0x1) == 0x1);

    // 57600 / (5 + 1) = 9600
    uart->BAUD = 5; 
    
    uart->CLKEN = 1;
    while ((uart->CLKEN & 0x1) == 0);

    // enable trx
    uart->ENABLE = 0x3;
    while ((uart->ENABLE & 0x3) != 0x3);
}

int32_t SER_put(uint8_t c)
{
    while ((uart->STATE & 0x1) == 0);
    uart->DATA = c;
    return c;
}

int main(void)
{
    GPIO_Init();
    DPLL_Init();
    SER_Init();
    
    // systick at 1000Hz
    SysTick_Config(4000L);              
    
    while (1) {
        SER_put('A');
        delay_ms(1000);
    }
}
