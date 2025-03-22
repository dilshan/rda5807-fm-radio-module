//-----------------------------------------------------------------------------
// Micro FM radio module firmware for STM8S003F3P6.
// 74HC595 based seven segment display driver.
//  
// Last updated: Dilshan Jayakody [26th Dec 2020]
//
// Update log:
// [26/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "include/stm8.h"
#include "include/stm8-util.h"

#include "serialssd.h"

// PC3 : Serial Data.
// PC4 : Latch Data.
// PC5 : Serial Clock.

void setDigitValue(unsigned char ssdValue)
{
    unsigned char pos = 7;

    while(pos != 0xFF)
    {
        // Set next available bit into data terminal and clear SRCLK, CLK.
        PC_ODR &= 0xC7;        
        PC_ODR |= (ssdValue & (1 << pos)) ? 0x08 : 0x00;
        delay_cycle(10);

        // Load data into SSD shift-register.
        PC_ODR |= 0x20;
        delay_cycle(20);

        pos--;
    }

    // Load ssdValue into output register.
    PC_ODR &= 0xC7;
    PC_ODR |= 0x10;    
}

unsigned char getDigitValue(unsigned char num, unsigned char clearOnZero)
{
    switch (num)
    {
    case 0:
        return (clearOnZero ? 0x00 : 0x3F);    // Character : 0
    case 1:
        return 0x06;    // Character : 1
    case 2:
        return 0x5B;    // Character : 2
    case 3:
        return 0x4F;    // Character : 3
    case 4:
        return 0x66;    // Character : 4
    case 5:
        return 0x6D;    // Character : 5
    case 6:
        return 0x7D;    // Character : 6
    case 7:
        return 0x07;    // Character : 7
    case 8:
        return 0x7F;    // Character : 8
    case 9:
        return 0x6F;    // Character : 9
    case 46:
        return 0x80;    // Character : dot
    default:
        return 0x00;    // Clear segment.
    }
}