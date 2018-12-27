#include "gct301s.h"

volatile uint32_t SYSTICK = 0;
uint32_t SW_ST = 0;

void SysTick_Handler(void)
{
    (void)SysTick->CTRL;  // read to clear flag
    SYSTICK++;
   
    if (GPIOA->DATAIN & 0x2) {  // switch (GPIO1) pressed?
        GPIOA->DATAOUT = 0x0;
        SYSTICK = 0;
        SW_ST = 0;
    }
    else {
        if (SW_ST == 0) {   // if switch was released, turn on LED immediately
            GPIOA->DATAOUT = 0x1;
            SYSTICK = 0;
            SW_ST = 1;
        }
        else if (SYSTICK >= 500) {
            GPIOA->DATAOUT ^= 0x1;
            SYSTICK = 0;            
        }
    }
}

int main(void)
{
    GPIOA->OUTENSET = 0x1;  // GPIO0 as output
    
    GPIOA->OUTENCLR = 0x2;  // GPIO1 as input
    GPIOA->PUDISCLR = 0x2;  // GPIO1 pull-up NOT disabled
    
    SysTick_Config(4000UL); // CPU at 4MHz, tick at 1kHz
    
    while (1);
}

