#include "gct301s.h"

volatile uint32_t SYSTICK = 0;
uint32_t PREV_SYSTICK = 0;

void SysTick_Handler(void)
{
    (void)SysTick->CTRL;  // read to clear flag
    SYSTICK++;
}

int main(void)
{
    GPIOA->OUTENSET = 0x1;
    SysTick_Config(4000UL); // CPU at 4MHz, tick at 1kHz
    
    while (1) {
        if ((SYSTICK - PREV_SYSTICK) >= 500) {
            PREV_SYSTICK = SYSTICK;
            GPIOA->DATAOUT ^= 0x1;
        }
    }
}
