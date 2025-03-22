//-----------------------------------------------------------------------------
// STM8S UART library for serial communication.
//
// Last updated: Dilshan Jayakody [2nd Nov 2020]
//
// Update log:
// [02/11/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "../include/stm8-uart.h"

void uartInit()
{
    // Set Baud rate with relative to master clock frequency.
    UART1_BRR2 = BAUD_RATE2;
    UART1_BRR1 = BAUD_RATE1;

    UART1_CR2 = UART1_CR2_TEN | UART1_CR2_REN;
}

void uartWrite(unsigned char data)
{
    UART1_DR = data;

    // Wait until data is transfered to host terminal.
    while (!(UART1_SR & UART1_SR_TC));
}

unsigned char uartRead()
{
    // Wait for incoming data from host terminal.
    while (!(UART1_SR & UART1_SR_RXNE));

    return UART1_DR;
}