#include <LPC214x.h>

void UART0_Init(void);
void UART0_SendChar(char ch);
void UART0_SendString(char *str);
char UART0_ReceiveChar(void);

void UART0_Init(void)
{
    // Select UART0 function
    // P0.0 = TXD0
    // P0.1 = RXD0
    PINSEL0 = 0x00000005;

    // Set Peripheral Clock = CCLK = 12 MHz
    VPBDIV = 0x01;

    // 8-bit data, 1 stop bit, no parity
    // Enable DLAB to set baud rate
    U0LCR = 0x83;

    // Set baud rate = 9600
    // PCLK = 12 MHz
    U0DLL = 78;
    U0DLM = 0;

    // Disable DLAB
    U0LCR = 0x03;
}

void UART0_SendChar(char ch)
{
    // Wait until transmitter is ready
    while (!(U0LSR & 0x20));

    // Send character
    U0THR = ch;
}

void UART0_SendString(char *str)
{
    while (*str)
    {
        UART0_SendChar(*str);
        str++;
    }
}

char UART0_ReceiveChar(void)
{
    // Wait until data is received
    while (!(U0LSR & 0x01));

    // Return received character
    return U0RBR;
}

int main(void)
{
    char data;

    // Initialize UART0
    UART0_Init();

    // Initial message
    UART0_SendString("UART Initialized\r\n");
    UART0_SendString("Type a character: ");

    while (1)
    {
        // Receive character
        data = UART0_ReceiveChar();

        // Display received character
        UART0_SendString("\r\nReceived: ");
        UART0_SendChar(data);

        // Ask for another character
        UART0_SendString("\r\nType a character: ");
    }
}