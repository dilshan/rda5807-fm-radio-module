//-----------------------------------------------------------------------------
// Micro FM radio module firmware for STM8S003F3P6.
// Main source file.
// 
// Last updated: Dilshan Jayakody [26th Dec 2020]
//
// Update log:
// [26/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef FM_MICRO_MAIN_HEADER
#define FM_MICRO_MAIN_HEADER

#define MODE_RESET_TIME     350
#define TUNER_SAVE_TIME     1000
#define MEM_MANAGER_IDLE_TIME   2000

enum SystemMode
{
    mdFreq,
    mdVolume,
    mdMemory
};

unsigned char displayValue[4] = {0xFF, 0xFF, 0xFF, 0xFF};
unsigned char displayDecimal;
unsigned char volumeLevel;
unsigned short modeResetCounter;
unsigned short lastCheckChannel;
enum SystemMode currentMode;

void initSystem();
void initDisplayTimer();
void setVolumeOnDisplay();
unsigned short getSavedTunerChannel();
unsigned char memoryManager(unsigned char *stationNumber, unsigned short *channel);
void isPresetChannel(unsigned short *currentChannel);

#endif /* FM_MICRO_MAIN_HEADER */