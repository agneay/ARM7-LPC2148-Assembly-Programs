#include <LPC21xx.h>

/* LCD Pins */
#define RS (1 << 16)
#define EN (1 << 17)

#define D4 (1 << 18)
#define D5 (1 << 19)
#define D6 (1 << 20)
#define D7 (1 << 21)

void delay(unsigned int count)
{
    unsigned int i, j;

    for(i = 0; i < count; i++)
    {
        for(j = 0; j < 1000; j++);
    }
}

/* Send data to LCD */
void lcd_data(unsigned char data)
{
    IOCLR0 = D4 | D5 | D6 | D7;

    if(data & 0x10)
        IOSET0 = D4;

    if(data & 0x20)
        IOSET0 = D5;

    if(data & 0x40)
        IOSET0 = D6;

    if(data & 0x80)
        IOSET0 = D7;

    IOSET0 = RS;

    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    IOCLR0 = D4 | D5 | D6 | D7;

    if(data & 0x01)
        IOSET0 = D4;

    if(data & 0x02)
        IOSET0 = D5;

    if(data & 0x04)
        IOSET0 = D6;

    if(data & 0x08)
        IOSET0 = D7;

    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    delay(2);
}

/* Send command to LCD */
void lcd_cmd(unsigned char cmd)
{
    IOCLR0 = RS;

    IOCLR0 = D4 | D5 | D6 | D7;

    if(cmd & 0x10)
        IOSET0 = D4;

    if(cmd & 0x20)
        IOSET0 = D5;

    if(cmd & 0x40)
        IOSET0 = D6;

    if(cmd & 0x80)
        IOSET0 = D7;

    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    IOCLR0 = D4 | D5 | D6 | D7;

    if(cmd & 0x01)
        IOSET0 = D4;

    if(cmd & 0x02)
        IOSET0 = D5;

    if(cmd & 0x04)
        IOSET0 = D6;

    if(cmd & 0x08)
        IOSET0 = D7;

    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    delay(2);
}

void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}

void lcd_init(void)
{
    IODIR0 |= RS | EN | D4 | D5 | D6 | D7;

    delay(20);

    lcd_cmd(0x02);

    lcd_cmd(0x28);

    lcd_cmd(0x0C);

    lcd_cmd(0x06);

    lcd_cmd(0x01);

    delay(5);
}

void lcd_number(unsigned int num)
{
    lcd_data((num / 10) + '0');

    lcd_data((num % 10) + '0');
}

/* RTC Initialization */
void rtc_init(void)
{
    CCR = 0x00;

    PREINT = 0x000001C7;
    PREFRAC = 0x000061C0;

    SEC = 0;
    MIN = 0;
    HOUR = 12;

    DOM = 19;
    MONTH = 8;
    YEAR = 2026;

    CCR = 0x01;
}

int main(void)
{
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;

    lcd_init();

    rtc_init();

    while(1)
    {
        /* Display Time */

        lcd_cmd(0x80);

        lcd_string("TIME:");

        lcd_number(HOUR);

        lcd_data(':');

        lcd_number(MIN);

        lcd_data(':');

        lcd_number(SEC);


        /* Display Date */

        lcd_cmd(0xC0);

        lcd_string("DATE:");

        lcd_number(DOM);

        lcd_data('/');

        lcd_number(MONTH);

        lcd_data('/');

        lcd_number(YEAR % 100);

        delay(200);
    }
}