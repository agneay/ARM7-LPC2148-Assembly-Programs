#include <LPC214x.h>

void timer0_delay(unsigned int delay_ms)
{
    T0TCR = 0x02;          // Reset Timer0

    T0PR = 14999;          // Prescaler for 1 ms delay
    T0TC = 0;              // Clear Timer Counter

    T0MR0 = delay_ms;      // Match value

    T0MCR = 0x04;          // Stop timer when TC = MR0

    T0TCR = 0x01;          // Start Timer0

    while(!(T0IR & 0x01)); // Wait until match occurs

    T0IR = 0x01;           // Clear interrupt flag
}

int main()
{
    // Configure P0.10 as GPIO
    PINSEL0 &= ~(3 << 20);

    // Configure P0.10 as output
    IO0DIR |= (1 << 10);

    while(1)
    {
        IO0SET = (1 << 10);    // LED ON
        timer0_delay(1000);    // 1 second delay

        IO0CLR = (1 << 10);    // LED OFF
        timer0_delay(1000);    // 1 second delay
    }
}