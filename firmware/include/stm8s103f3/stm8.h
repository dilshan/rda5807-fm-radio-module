//-----------------------------------------------------------------------------
// STM8S103F2, STM8S103F3, STM8S103K3 header file.
//
// Based on STM8S103 datasheet DocID15441 Rev 14.
// Last updated: Dilshan Jayakody [7th Oct 2020]
//
// Update log:
// [07/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef STM8S103_MAIN_H
#define STM8S103_MAIN_H

#include "stm8s103.h"
#include "interrupt.h"

#include "adc1def.h"
#include "beeperdef.h"
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

#endif /* STM8S103_MAIN_H */