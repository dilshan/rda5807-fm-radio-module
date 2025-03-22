//-----------------------------------------------------------------------------
// Micro FM radio module firmware for STM8S003F3P6.
// RDA5807M driver source file. 
// (Based on datasheet Rev 1.8 - Aug.2014)
//
// Last updated: Dilshan Jayakody [26th Dec 2020]
//
// Update log:
// [26/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "include/stm8-i2c.h"
#include "include/stm8-util.h"
#include "rda5807m.h"

#define RDA5807M_WRITE_ADDRESS 0x20
#define RDA5807M_READ_ADDRESS 0x21

#define TUNE_SEEK_DOWN  0x00
#define TUNE_SEEK_UP    0x02

#define SEEK_CHANNEL    0x01
#define TUNE_RECEIVER   0x10

#define MIN_FREQ        87000UL
#define CHANNEL_SPACE   25

unsigned char rdaWriteReg[RECEIVER_WRITE_CONFIG_LEN] = {
    INIT_RX_REG_0, INIT_RX_REG_1,   // 02H : Write operation starts from this address.    
    INIT_RX_REG_2, INIT_RX_REG_3,   // 03H
    INIT_RX_REG_4, INIT_RX_REG_5,   // 04H 
    INIT_RX_REG_6, INIT_RX_REG_7,   // 05H
    INIT_RX_REG_8, INIT_RX_REG_9,   // 06H 
    INIT_RX_REG_10, INIT_RX_REG_11  // 07H
    };

unsigned char rdaReadReg[RECEIVER_READ_CONFIG_LEN] = {
    0, 0,   // 0AH - RDSR | STC | SF | RDSS | BLK_E | ST | READCHAN | READCHAN
            //       READCHAN
    0, 0    // 0BH - RSSI | RSSI | RSSI | RSSI | RSSI | RSSI | RSSI | FM TRUE
            //       FM_READY | RSVD | RSVD | ABCD_E | BLERA | BLERA | BLERB | BLERB
    };

void initRDAFMReceiver(unsigned char volLvl, unsigned short channel)
{   
    // Initialize I2C interface of the MCU.
    i2cInit();
    delay_cycle(75);

    // Setup RDA5807M regsiter values to initialize the controller.
    if(volLvl == 0)
    {
        // At level 0 mute the audio output.
        rdaWriteReg[0] &= 0xBF;
    }
    else
    {
        // Release the mute state of the audio output and set DAC gain.
        rdaWriteReg[0] |= 0x40;
        rdaWriteReg[7] &= 0xF0;
        rdaWriteReg[7] |= ((volLvl - 1) & 0x0F);
    }

    // Tune receiver into the specified channel.
    rdaWriteReg[2] = (channel >> 2);
    rdaWriteReg[3] |= (((channel << 6) & 0xC0) | TUNE_RECEIVER); 

    // Initialize RDA5807M receiver.
    updateReceiverConfig(RECEIVER_WRITE_CONFIG_LEN);
    delay_ms(5);

    // Clear reset flag.
    rdaWriteReg[1] &= 0xFD;
    updateReceiverConfig(RECEIVER_WRITE_CONFIG_LEN);

    // Relase tune flag and tune frequency.
    rdaWriteReg[2] = 0;
    rdaWriteReg[3] &= 0x2F;
}

void setTunerFrequency(unsigned short channel)
{    
    // Clear seek bit to ensure the frequency tuning.
    rdaWriteReg[0] &= 0xFE;

    // Tune receiver into the specified channel.
    rdaWriteReg[2] = (channel >> 2);
    rdaWriteReg[3] |= (((channel << 6) & 0xC0) | TUNE_RECEIVER); 

    updateReceiverConfig(4);

    // Clear receiver tune flag.
    rdaWriteReg[2] = 0x00;
    rdaWriteReg[3] &= 0x2F;
}

void getTunerFrequency(unsigned char *freq)
{
    // Extract channel data from the receive buffer.
    unsigned long tunedFreq = ((rdaReadReg[0] << 8) | rdaReadReg [1]) & 0x3FF;
    tunedFreq = ((tunedFreq * CHANNEL_SPACE) + MIN_FREQ) / 100;

    // Convert frequency into 4 digit number.
    freq[0] = tunedFreq / 1000;
    freq[1] = (tunedFreq % 1000) / 100;
    freq[2] = (tunedFreq % 100) / 10;
    freq[3] = tunedFreq % 10;
}

void getTunerChannel(unsigned short *channel)
{
    (*channel) = ((rdaReadReg[0] << 8) | rdaReadReg [1]) & 0x3FF;
}

unsigned char isStation()
{
    return ((rdaReadReg[2]) & 0x01);
}

unsigned char isStereoChannel()
{    
    // Get stereo status only from the FM stations.
    return (((rdaReadReg[2]) & 0x01) && ((rdaReadReg[0]) & 0x04));
}

void updateReceiverConfig(unsigned char length)
{
    unsigned char pos = 0;

    // Start update RDA5807M registers.    
    i2cStart();
    i2cWriteAddr(RDA5807M_WRITE_ADDRESS);

    // Send specified configuration bytes to the receiver.
    while(pos < length)
    {
        i2cWrite(rdaWriteReg[pos]);
        pos++;
    }

    i2cStop();
}

void getReceiverConfig(unsigned char length)
{
    unsigned char pos = 0;
    
    // Start update RDA5807M registers.    
    i2cStart();
    i2cWriteAddr(RDA5807M_READ_ADDRESS);

    // Read specified number of bytes from RDA5807M registers.
    while(pos < (length - 1))
    {
        rdaReadReg[pos] = i2cRead(1);
        pos++;
    }

    // Read last configuration byte with NACK.
    rdaReadReg[pos] = i2cRead(0);
    i2cStop();
}

void seekChannel(unsigned char isSeekUp)
{
    // Set SEEK and SEEKUP bits to start the channel search.
    rdaWriteReg[0] |= (SEEK_CHANNEL | (isSeekUp ? TUNE_SEEK_UP : TUNE_SEEK_DOWN));
    updateReceiverConfig(4);

    // Restore seek and seek direction bits to defaults.
    rdaWriteReg[0] &= 0xFC;
}

void setRDAVolume(unsigned char volLvl)
{
    if(volLvl == 0)
    {
        // At level 0 mute the audio output.
        rdaWriteReg[0] &= 0xBF;
    }
    else
    {
        // Release the mute state of the audio output and set DAC gain.
        rdaWriteReg[0] |= 0x40;
        rdaWriteReg[7] &= 0xF0;
        rdaWriteReg[7] |= ((volLvl - 1) & 0x0F);
    }
    
    updateReceiverConfig(8);
}