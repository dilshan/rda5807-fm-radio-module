//-----------------------------------------------------------------------------
// STM8S common utility functions.
// 
// Last updated: Dilshan Jayakody [12th Oct 2020]
//
// Update log:
// [07/10/2020] - Initial version - Dilshan Jayakody.
// [12/10/2020] - Add inline delay option - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "../include/stm8-util.h"

#ifndef INLINE_DELAY

void delay_ms(unsigned long ms)
{
    unsigned long countdown;

    for (countdown = 0; countdown < ((F_CPU / 18 / 1000UL) * ms); countdown++)
    {
        nop();
    }
}

void delay_cycle(unsigned short cycle)
{
    while(cycle)
    {
        cycle--;
    }
}

#endif