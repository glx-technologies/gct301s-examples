/*
** Extend the uart_poll_tx to echo the received character.
**
*/

#include "gct301s.h"

//-----------------------------------------------------------------------------
// Select UART to use
#define USE_UART    5

//-----------------------------------------------------------------------------
#if (USE_UART==0)
UART_TypeDef *uart = UART0;
#elif (USE_UART==1)
UART_TypeDef *uart = UART1;
#elif (USE_UART==2)
UART_TypeDef *uart = UART2;
#elif (USE_UART==3)
UART_TypeDef *uart = UART3;
#elif (USE_UART==4)
UART_TypeDef *uart = UART4;
#elif (USE_UART==5)
UART_TypeDef *uart = UART5;
#else
UART_TypeDef *uart = UART0;
#endif

volatile uint32_t SYSTICK = 0;

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
void GPIO_Init(void)
{
#if (USE_UART==0)
    GPIOC->ALTFUNCSET = BV(1) | BV(0);
#elif (USE_UART==1)
    GPIOC->ALTFUNCSET = BV(3) | BV(2);
#elif (USE_UART==2)
    GPIOC->ALTFUNCSET = BV(5) | BV(4);
#elif (USE_UART==3)
    GPIOA->ALTFUNCSET = BV(11) | BV(10);    // GPIOA
#elif (USE_UART==4)
    GPIOC->ALTFUNCSET = BV(7) | BV(6);
#elif (USE_UART==5)
    GPIOA->ALTFUNCSET = BV(7) | BV(6);      // GPIOA
#else
    GPIOC->ALTFUNCSET = BV(1) | BV(0);
#endif    
    
}

//-----------------------------------------------------------------------------
void DPLL_Init(void)
{
    // turn on DPLL
    SYSCTRL->DPLL_CTL = 0x1;
    while ((SYSCTRL->SYS_CLK_STA & 0x1) == 0);
}

//-----------------------------------------------------------------------------
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

int32_t SER_get(void)
{
    while ((uart->STATE & 0x2) == 0);
    return uart->DATA;
}

//-----------------------------------------------------------------------------
int main(void)
{
    // initialize GPIO for TX0 & RX0
    GPIO_Init();
    
    // Enable DPLL for uart clock
    DPLL_Init();
    
    // Configure and enable uart
    SER_Init();
    
    // systick at 1000Hz
    SysTick_Config(4000L);              
    
    while (1) {
        SER_put(SER_get());
    }
}
