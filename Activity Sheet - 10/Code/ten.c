#include <LPC214x.h>

void EINT0_ISR(void) __irq
{
    IO0PIN ^= (1 << 16);      // Toggle LED

    EXTINT = (1 << 0);        // Clear EINT0 interrupt flag
    VICVectAddr = 0;          // End of interrupt
}

int main(void)
{
    // Configure P0.16 as GPIO
    PINSEL1 &= ~(3 << 0);     // P0.16 = GPIO

    // Configure P0.16 as output
    IODIR0 |= (1 << 16);

    // Initial LED state
    IO0CLR = (1 << 16);       // LED OFF

    // Configure P0.1 as EINT0
    PINSEL0 &= ~(3 << 2);
    PINSEL0 |=  (3 << 2);

    // Configure EINT0 as edge-triggered
    EXTMODE |= (1 << 0);

    // Falling-edge triggered
    EXTPOLAR &= ~(1 << 0);

    // Clear any pending EINT0 interrupt
    EXTINT = (1 << 0);

    // Configure VIC
    VICIntSelect &= ~(1 << 14);       // EINT0 as IRQ
    VICVectAddr0 = (unsigned long)EINT0_ISR;
    VICVectCntl0 = (1 << 5) | 14;     // Enable slot 0, source EINT0
    VICIntEnable = (1 << 14);         // Enable EINT0 interrupt

    while(1)
    {
        // Wait for interrupt
    }
}