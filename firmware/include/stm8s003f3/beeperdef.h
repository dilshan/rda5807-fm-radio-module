//-----------------------------------------------------------------------------
// STM8S003F3, STM8S003K3 Beeper definitions for SDCC.
//
// Based on STM8S003 datasheet DS7147 Rev 10.
// Last updated: Dilshan Jayakody [4th Dec 2020]
//
// Update log:
// [04/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef STM8S003_BEEPER_DEF_H
#define STM8S003_BEEPER_DEF_H

#define BEEP_CSR_RESET_VALUE    ((unsigned char)0x1F)   // CSR reset value.

#define BEEP_CSR_BEEPSEL    ((unsigned char)0xC0)   // Beeper frequency selection mask.
#define BEEP_CSR_BEEPEN     ((unsigned char)0x20)   // Beeper enable mask.
#define BEEP_CSR_BEEPDIV    ((unsigned char)0x1F)   // Beeper Divider prescalar mask.

#endif /* STM8S003_BEEPER_DEF_H */