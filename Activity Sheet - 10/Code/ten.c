#include <LPC214x.h>

void EINT0_ISR(void) __irq
{
    IO0PIN ^= (1 << 16);     // Toggle LED

    EXTINT = (1 << 0);       // Clear EINT0 interrupt flag
    VICVectAddr = 0;         // End of interrupt
}

int main(void)
{
    // Configure P0.16 as GPIO output for LED
    PINSEL1 = 0x00000000;
    IODIR0 |= (1 << 16);

    // Configure P0.1 as EINT0
    PINSEL0 |= (3 << 2);

    // Configure EINT0 as edge-triggered
    EXTMODE |= (1 << 0);

    // Falling-edge interrupt
    EXTPOLAR &= ~(1 << 0);

    // Configure VIC for EINT0
    VICIntSelect &= ~(1 << 14);      // IRQ mode
    VICVectAddr0 = (unsigned long)EINT0_ISR;
    VICVectCntl0 = (1 << 5) | 14;    // Enable slot 0 for EINT0
    VICIntEnable = (1 << 14);        // Enable EINT0 interrupt

    // LED initially OFF
    IO0SET = (1 << 16);

    while(1)
    {
        // Main program does nothing
        // LED changes only when interrupt occurs
    }
}