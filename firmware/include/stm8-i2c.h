//-----------------------------------------------------------------------------
// STM8S I2C library for standard mode communication.
//
// Last updated: Dilshan Jayakody [9th Oct 2020]
//
// Update log:
// [09/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef HARDWARE_I2C_H
#define HARDWARE_I2C_H

#include "../include/stm8.h"

void i2cInit();

void i2cStart();
void i2cStop();

void i2cWriteAddr(unsigned char addr);
void i2cWrite(unsigned char data);

unsigned char i2cRead(unsigned char ack);

#endif /* HARDWARE_I2C_H */