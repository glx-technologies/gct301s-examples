#include "gct301s.h"

void delay(uint32_t d)
{
    while(d--) {
        __NOP();
    }
}

int main(void)
{
    GPIOA->OUTENSET = 0x1;
    
    while (1) {
        GPIOA->DATAOUT = 0x1;
        delay(250000);
        GPIOA->DATAOUT = 0x0;
        delay(250000);
    }
}

