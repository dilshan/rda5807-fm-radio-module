//-----------------------------------------------------------------------------
// Micro FM radio module firmware for STM8S003F3P6.
// 74HC595 based seven segment display driver.
// 
// Last updated: Dilshan Jayakody [26th Dec 2020]
//
// Update log:
// [26/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef FM_MICRO_SERIAL_SSD_HEADER
#define FM_MICRO_SERIAL_SSD_HEADER

void setDigitValue(unsigned char ssdValue);
unsigned char getDigitValue(unsigned char num, unsigned char clearOnZero);

#endif /* FM_MICRO_SERIAL_SSD_HEADER */