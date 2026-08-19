#include <LPC213x.h>

#define RS (1 << 16)
#define EN (1 << 17)

unsigned int adc_value;

void delay(unsigned int count)
{
    unsigned int i, j;

    for(i = 0; i < count; i++)
        for(j = 0; j < 100; j++);
}

/* Send command to LCD */
void lcd_cmd(unsigned char cmd)
{
    IOCLR0 = RS;

    /* Send higher nibble */
    IOCLR0 = 0x003C0000;       // Clear P0.18 to P0.21
    IOSET0 = ((cmd & 0xF0) << 14);

    IOSET0 = EN;
    delay(10);
    IOCLR0 = EN;

    /* Send lower nibble */
    IOCLR0 = 0x003C0000;
    IOSET0 = ((cmd & 0x0F) << 18);

    IOSET0 = EN;
    delay(10);
    IOCLR0 = EN;
}

/* Send data to LCD */
void lcd_data(unsigned char data)
{
    IOSET0 = RS;

    /* Send higher nibble */
    IOCLR0 = 0x003C0000;
    IOSET0 = ((data & 0xF0) << 14);

    IOSET0 = EN;
    delay(10);
    IOCLR0 = EN;

    /* Send lower nibble */
    IOCLR0 = 0x003C0000;
    IOSET0 = ((data & 0x0F) << 18);

    IOSET0 = EN;
    delay(10);
    IOCLR0 = EN;
}

/* Display string */
void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}

/* Display ADC number */
void lcd_number(unsigned int num)
{
    unsigned char thousands, hundreds, tens, ones;

    thousands = num / 1000;
    num = num % 1000;

    hundreds = num / 100;
    num = num % 100;

    tens = num / 10;
    ones = num % 10;

    if(thousands)
        lcd_data(thousands + '0');

    if(thousands || hundreds)
        lcd_data(hundreds + '0');

    if(thousands || hundreds || tens)
        lcd_data(tens + '0');

    lcd_data(ones + '0');
}

/* Initialize LCD */
void lcd_init(void)
{
    delay(100);

    lcd_cmd(0x02);     // 4-bit mode
    lcd_cmd(0x28);     // 2 lines, 5x7 matrix
    lcd_cmd(0x0C);     // Display ON, cursor OFF
    lcd_cmd(0x06);     // Increment cursor
    lcd_cmd(0x01);     // Clear display

    delay(100);
}

int main(void)
{
    /* LCD pins P0.16 to P0.21 as GPIO */
    PINSEL1 = 0x01000000;

    /* P0.16 to P0.21 as output */
    IODIR0 |= 0x003F0000;

    lcd_init();

    lcd_cmd(0x80);
    lcd_string("ADC VALUE:");

    while(1)
    {
        /* Configure and enable ADC0 Channel 1 */
        AD0CR = (1 << 1) | (10 << 8) | (1 << 21);

        /* Start ADC conversion */
        AD0CR |= (1 << 24);

        /* Wait for conversion */
        while(!(AD0GDR & (1UL << 31)));

        /* Extract 10-bit ADC value */
        adc_value = (AD0GDR >> 6) & 0x3FF;

        /* Move to second line */
        lcd_cmd(0xC0);

        /* Display ADC value */
        lcd_number(adc_value);

        delay(500);
    }
}