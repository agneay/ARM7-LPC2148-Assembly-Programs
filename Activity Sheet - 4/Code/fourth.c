#include <LPC213x.h>

#define RS (1 << 8)
#define EN (1 << 9)

void delay(unsigned int count)
{
    unsigned int i, j;

    for(i = 0; i < count; i++)
        for(j = 0; j < 1000; j++);
}

void lcd_command(unsigned char cmd)
{
    // Clear old data from P0.16 to P0.23
    IOCLR0 = 0x00FF0000;

    // Send command to P0.16-P0.23
    IOSET0 = ((unsigned int)cmd << 16);

    // RS = 0 for command
    IOCLR0 = RS;

    // Enable pulse
    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    delay(2);
}

void lcd_data(unsigned char data)
{
    // Clear old data
    IOCLR0 = 0x00FF0000;

    // Send data to P0.16-P0.23
    IOSET0 = ((unsigned int)data << 16);

    // RS = 1 for data
    IOSET0 = RS;

    // Enable pulse
    IOSET0 = EN;
    delay(1);
    IOCLR0 = EN;

    delay(2);
}

void lcd_string(char *str)
{
    while(*str != '\0')
    {
        lcd_data(*str);
        str++;
    }
}

void lcd_init(void)
{
    delay(20);

    lcd_command(0x38);   // 8-bit mode, 2 lines
    lcd_command(0x0C);   // Display ON, cursor OFF
    lcd_command(0x06);   // Auto increment cursor
    lcd_command(0x01);   // Clear LCD

    delay(2);
}

int main(void)
{
    // Configure pins as GPIO
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;

    // P0.8, P0.9 and P0.16-P0.23 as output
    IODIR0 = 0x00FF0300;

    lcd_init();

    // First line
    lcd_command(0x80);
    lcd_string("Hello!");

    // Second line
    lcd_command(0xC0);
    lcd_string("LPC2138 LCD");

    while(1);
}