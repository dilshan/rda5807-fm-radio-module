//-----------------------------------------------------------------------------
// STM8S EEPROM library for standard mode communication.
//
// Last updated: Dilshan Jayakody [24th Oct 2020]
//
// Update log:
// [24/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef HARDWARE_EEPROM_H
#define HARDWARE_EEPROM_H

#include "../include/stm8.h"

void eepromWrite(unsigned short addr, unsigned char value);

static inline unsigned char eepromRead(unsigned short addr)
{
    return GLOBAL_MEM(addr);
}

#endif /* HARDWARE_EEPROM_H */