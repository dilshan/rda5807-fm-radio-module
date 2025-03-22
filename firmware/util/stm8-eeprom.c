//-----------------------------------------------------------------------------
// STM8S EEPROM library for standard mode communication.
//
// Last updated: Dilshan Jayakody [24th Oct 2020]
//
// Update log:
// [24/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "../include/stm8-eeprom.h"

void eepromWrite(unsigned short addr, unsigned char value)
{
    // Unlock EEPROM.
    FLASH_DUKR = FLASH_DUKR_KEY1;
    FLASH_DUKR = FLASH_DUKR_KEY2;
    while (!(FLASH_IAPSR & FLASH_IAPSR_DUL));

    // Writing data into specified address.
    GLOBAL_MEM(addr) = value;
    while (!(FLASH_IAPSR & FLASH_IAPSR_EOP));

    // Lock EEPROM.
    FLASH_IAPSR &= ~(FLASH_IAPSR_DUL);
}