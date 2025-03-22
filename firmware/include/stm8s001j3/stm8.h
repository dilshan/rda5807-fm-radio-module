//-----------------------------------------------------------------------------
// STM8S001J3 header file.
//
// Based on STM8S001J3 datasheet DS12129 Rev 4.
// Last updated: Dilshan Jayakody [15th Nov 2020]
//
// Update log:
// [01/11/2020] - Initial version - Dilshan Jayakody.
// [15/11/2020] - Add EEPROM support - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef STM8S001_MAIN_H
#define STM8S001_MAIN_H

#include "stm8s001.h"
#include "interrupt.h"

#include "adc1def.h"
#include "i2cdef.h"
#include "spidef.h"
#include "uart1def.h"

#include "timer1def.h"
#include "timer2def.h"
#include "timer4def.h"

#include "flash.h"

// EEPROM start and end addresses.
#define EEPROM_START_ADDR       0x4000
#define EEPROM_END_ADDR         0x407F

// MASS keys for FLASH_DUKR register to disable the DATA area write protection.
#define FLASH_DUKR_KEY1     0xAE
#define FLASH_DUKR_KEY2     0x56

#endif /* STM8S001_MAIN_H */