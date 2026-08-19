#include <LPC213x.h>

int main(void)
{
    /* Select PWM4 function on P0.7 */
    PINSEL0 &= ~(3 << 14);
    PINSEL0 |=  (2 << 14);

    /* Reset PWM counter */
    PWMTCR = 0x02;

    PWMPR = 0;

    /* PWM period */
    PWMMR0 = 59999;

    /* 50% duty cycle */
    PWMMR4 = 30000;

    /* Reset counter when MR0 matches */
    PWMMCR = 0x02;

    /* Enable PWM4 output */
    PWMPCR = (1 << 12);

    /* Latch MR0 and MR4 */
    PWMLER = (1 << 0) | (1 << 4);

    /* Enable PWM and counter */
    PWMTCR = 0x09;

    while(1);
}