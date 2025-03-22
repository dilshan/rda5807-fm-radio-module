//-----------------------------------------------------------------------------
// STM8S UART library for serial communication.
//
// Last updated: Dilshan Jayakody [2nd Nov 2020]
//
// Update log:
// [02/11/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef HARDWARE_UART1_H
#define HARDWARE_UART1_H

#include "../include/stm8.h"

#ifndef BAUD_RATE1  
#warning "BAUD rate 1 is not defined. Use standard BAUD rate 9600 (CLK = 2MHz)."
#define BAUD_RATE1 0x0D
#endif

#ifndef BAUD_RATE2  
#warning "BAUD rate 2 is not defined. Use standard BAUD rate 9600 (CLK = 2MHz)."
#define BAUD_RATE2 0x01
#endif

void uartInit();
void uartWrite(unsigned char data);
unsigned char uartRead();

#define uartGetByte() (UART1_DR)

static inline unsigned char uartDataAvailable()
{
    return (UART1_SR & UART1_SR_RXNE);
}

#endif /* HARDWARE_UART1_H */