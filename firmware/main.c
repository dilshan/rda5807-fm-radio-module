//-----------------------------------------------------------------------------
// Micro FM radio module firmware for STM8S003F3P6.
// Main source file.
// 
// Last updated: Dilshan Jayakody [26th Dec 2020]
//
// Update log:
// [26/12/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#include "include/stm8.h"
#include "include/stm8-util.h"
#include "include/stm8-eeprom.h"

#include "main.h"
#include "serialssd.h"
#include "rda5807m.h"

#define CLEAR_SSD_DECIMAL   0x00
#define SET_SSD_DECIMAL     0x80

#define MAX_VOLUME_LEVEL    0x10

// The maximum allowed station numbers are 19.
#define MAX_STATION_NUMBER  10
#define MIN_STATION_NUMBER  1

#define EEPROM_VOLUME_ADDRESS   (EEPROM_START_ADDR + 1)
#define EEPROM_TUNE_CHANNEL_ADDRESS   (EEPROM_START_ADDR + 2)
#define EEPROM_MEM_MANAGER_BASE   (EEPROM_START_ADDR + 4)

void TIM2_update() __interrupt(TIMER2_TRIGGER_IRQ)
{
    // Set value of the first digit.
    PD_ODR &= 0xE1;
    setDigitValue(getDigitValue(displayValue[0], 1));
    PD_ODR |= 0x02;
    delay_cycle(300);

    // Set value of the second digit.
    PD_ODR &= 0xE1;
    setDigitValue(getDigitValue(displayValue[1], 0));
    PD_ODR |= 0x04;
    delay_cycle(300);

    // Set value of the third digit.
    PD_ODR &= 0xE1;
    setDigitValue(getDigitValue(displayValue[2], 0) | displayDecimal);
    PD_ODR |= 0x08;
    delay_cycle(290);

    // Set value of the fourth digit.
    PD_ODR &= 0xE1;
    setDigitValue(getDigitValue(displayValue[3], 0));
    PD_ODR |= 0x10;
    delay_cycle(300);

    // Shutdown all segments.
    PD_ODR &= 0xE1;

    // Clear timer 2 interrupt flag.
    TIM2_SR1 &= ~TIM2_SR1_UIF;
}

void main()
{
    unsigned char buttonState;
    unsigned short tunerChannel;
    unsigned char animCycle;
    unsigned char memoryManagerStation;
    
    cli();

    // Initialize MCU peripherals and global variables.
    initSystem();
    delay_ms(40);

    // Activate display control timer (Timer2).
    initDisplayTimer();

    // Load last volume level from EEPROM.
    volumeLevel = eepromRead(EEPROM_VOLUME_ADDRESS);
    if(volumeLevel > MAX_VOLUME_LEVEL)
    {
        // Volume setting is invalid or first time load.
        volumeLevel = 0;
    }

    // Load last saved station from EEPROM.
    tunerChannel = getSavedTunerChannel();
    if(tunerChannel > 0x3FF)
    {
        // Tuner channel is invalid or first time load.
        tunerChannel = 0;
    }

    // Start seven segment display panel.
    sei();
    delay_ms(20);
 
    // Display startup animation (to provide time to initialize RDA5807M).
    animCycle = 0;
    while(animCycle < 18)
    {
        switch (animCycle % 6)
        {
        case 0:
            displayValue[0] = 0x2E;
            displayValue[1] = 0xFF;
            displayValue[2] = 0xFF;
            displayValue[3] = 0xFF;
            break;
        case 1:
        case 5:
            displayValue[0] = 0xFF;
            displayValue[1] = 0x2E;
            displayValue[2] = 0xFF;
            displayValue[3] = 0xFF;
            break;
        case 2:
        case 4:
            displayValue[0] = 0xFF;
            displayValue[1] = 0xFF;
            displayValue[2] = 0x2E;
            displayValue[3] = 0xFF;
            break;
        case 3:        
            displayValue[0] = 0xFF;
            displayValue[1] = 0xFF;
            displayValue[2] = 0xFF;
            displayValue[3] = 0x2E;
            break;
        }

        delay_ms(60);
        animCycle++;
    }

    // End of the startup animation by shutting down all the segments.
    displayValue[0] = 0xFF;
    displayValue[1] = 0xFF;
    displayValue[2] = 0xFF;
    displayValue[3] = 0xFF;
    delay_ms(100);

    // Initialize RDA5807M receiver.
    initRDAFMReceiver(volumeLevel, tunerChannel);

    buttonState = (PD_IDR & 0x60) | (PA_IDR & 0x0E);
    displayDecimal = SET_SSD_DECIMAL;
    memoryManagerStation = 1;

    // Get receiver information.
    getReceiverConfig(RECEIVER_READ_CONFIG_LEN);
    getTunerFrequency(displayValue);

    // Main service loop.
    while(1)
    {
        // Check for UP button event.
        if(((PD_IDR & 0x20) == 0x20) && ((buttonState & 0x20) == 0x00))
        {
            // Save any pending volume level changes.
            if(currentMode == mdVolume)
            {
                // Save new volume level in EEPROM.
                if(eepromRead(EEPROM_VOLUME_ADDRESS) != volumeLevel)
                {
                    eepromWrite(EEPROM_VOLUME_ADDRESS, volumeLevel);
                }
            }
            
            // UP button press is detected.
            displayDecimal = SET_SSD_DECIMAL;
            currentMode = mdFreq;

            // Set modeResetCounter to raise EEPROM save function.
            modeResetCounter = 1;

            seekChannel(0);
        }

        // Check for DOWN button event.
        if(((PD_IDR & 0x40) == 0x40) && ((buttonState & 0x40) == 0x00))
        {
            // Save any pending volume level changes.
            if(currentMode == mdVolume)
            {
                // Save new volume level in EEPROM.
                if(eepromRead(EEPROM_VOLUME_ADDRESS) != volumeLevel)
                {
                    eepromWrite(EEPROM_VOLUME_ADDRESS, volumeLevel);
                }
            }
            
            // DOWN button press is detected.
            displayDecimal = SET_SSD_DECIMAL;
            currentMode = mdFreq;

            // Set modeResetCounter to raise EEPROM save function.
            modeResetCounter = 1;

            seekChannel(1);
        }

        // Check for volume UP button event.
        if(((PA_IDR & 0x02) == 0x02) && ((buttonState & 0x02) == 0x00))
        {
            // Save any pending frequency changes.
            if((currentMode == mdFreq) && (modeResetCounter > 0))
            {
                // Save pending frequency changes into EEPROM.
                modeResetCounter = 0;
                getTunerChannel(&tunerChannel);

                if(tunerChannel != getSavedTunerChannel())
                {
                    // Save channel into EEPROM.
                    eepromWrite(EEPROM_TUNE_CHANNEL_ADDRESS, ((tunerChannel >> 8) & 0xFF));
                    eepromWrite((EEPROM_TUNE_CHANNEL_ADDRESS + 1), (tunerChannel & 0xFF));
                }
            }
            
            // Volume UP button press is detected.            
            volumeLevel = volumeLevel + ((volumeLevel == MAX_VOLUME_LEVEL) ? 0 : 1);
            displayDecimal = CLEAR_SSD_DECIMAL;
            currentMode = mdVolume;
            modeResetCounter = 0;

            setVolumeOnDisplay();
            setRDAVolume(volumeLevel);
        }

        // Check for volume DOWN button event.
        if(((PA_IDR & 0x04) == 0x04) && ((buttonState & 0x04) == 0x00))
        {
            // Save any pending frequency changes.
            if((currentMode == mdFreq) && (modeResetCounter > 0))
            {
                // Save pending frequency changes into EEPROM.
                modeResetCounter = 0;
                getTunerChannel(&tunerChannel);

                if(tunerChannel != getSavedTunerChannel())
                {
                    // Save channel into EEPROM.
                    eepromWrite(EEPROM_TUNE_CHANNEL_ADDRESS, ((tunerChannel >> 8) & 0xFF));
                    eepromWrite((EEPROM_TUNE_CHANNEL_ADDRESS + 1), (tunerChannel & 0xFF));
                }
            }
            
            // Volume DOWN button press is detected.
            volumeLevel = volumeLevel - ((volumeLevel == 0) ? 0 : 1);               
            displayDecimal = CLEAR_SSD_DECIMAL;
            currentMode = mdVolume;
            modeResetCounter = 0;        

            setVolumeOnDisplay();
            setRDAVolume(volumeLevel);            
        }

        // Check for memory manager button event.
        if(((PA_IDR & 0x08) == 0x08) && ((buttonState & 0x08) == 0x00))
        {
            // Save any pending frequency changes.
            if((currentMode == mdFreq) && (modeResetCounter > 0))
            {
                // Save pending frequency changes into EEPROM.
                modeResetCounter = 0;
                getTunerChannel(&tunerChannel);

                if(tunerChannel != getSavedTunerChannel())
                {
                    // Save channel into EEPROM.
                    eepromWrite(EEPROM_TUNE_CHANNEL_ADDRESS, ((tunerChannel >> 8) & 0xFF));
                    eepromWrite((EEPROM_TUNE_CHANNEL_ADDRESS + 1), (tunerChannel & 0xFF));
                }
            }            
            else
            {
                // Save any pending volume level changes.
                if(eepromRead(EEPROM_VOLUME_ADDRESS) != volumeLevel)
                {
                    eepromWrite(EEPROM_VOLUME_ADDRESS, volumeLevel);
                }
            }
            
            // Get current channel from the tuner.
            modeResetCounter = 0;
            getTunerChannel(&tunerChannel);

            // Activate memory manager.
            if(memoryManager(&memoryManagerStation, &tunerChannel))
            {
                // Station has been recall by the memory manager.
                setTunerFrequency(tunerChannel);
                delay_ms(50);

                // Set modeResetCounter to raise EEPROM save function.
                modeResetCounter = 1;
            }   

            // Reset preset indicator flag to perform force scan on memory manager's channels.
            lastCheckChannel = 0xFFFF;

            // Return to frequency mode.
            getReceiverConfig(RECEIVER_READ_CONFIG_LEN);
            getTunerFrequency(displayValue);
            displayDecimal = SET_SSD_DECIMAL;
            currentMode = mdFreq;
        }

        buttonState = (PD_IDR & 0x60) | (PA_IDR & 0x0E);
        
        // Get current frequency and status data from the RDA receiver.
        getReceiverConfig(RECEIVER_READ_CONFIG_LEN);

        // Update stereo indicator.
        if(isStereoChannel())
        {
            // Stereo channel.
            PC_ODR |= 0x80;
        }
        else
        {
            // Mono channel.
            PC_ODR &= 0x7F;
        }

        // Control preset channel indicator (to identify saved channels).
        if(isStation())
        {
            getTunerChannel(&tunerChannel);
            isPresetChannel(&tunerChannel);
        }
        else
        {
            // Shutdown preset channel indicator because current channel is not a station.
            PC_ODR &= 0xBF;
        }
                
        // Reset mode counter.
        if(currentMode == mdVolume)
        {                        
            // System is in volume mode.
            if((++modeResetCounter) > MODE_RESET_TIME)
            {
                // Save new volume level in EEPROM.
                if(eepromRead(EEPROM_VOLUME_ADDRESS) != volumeLevel)
                {
                    eepromWrite(EEPROM_VOLUME_ADDRESS, volumeLevel);
                }
                
                // Mode timeout reached and set system (display) mode to Frequency Mode.
                modeResetCounter = 0;
                displayDecimal = SET_SSD_DECIMAL;
                currentMode = mdFreq;
            }
        }
        else
        {
            // System is in Frequency mode.            
            getTunerFrequency(displayValue);

            // If channel is changed, check for save timeout.
            if(modeResetCounter > 0)
            {
                if((++modeResetCounter) > TUNER_SAVE_TIME)
                {
                    // Channel save timeout is reached.
                    modeResetCounter = 0;
                    getTunerChannel(&tunerChannel);

                    if(tunerChannel != getSavedTunerChannel())
                    {
                        // Save channel into EEPROM.
                        eepromWrite(EEPROM_TUNE_CHANNEL_ADDRESS, ((tunerChannel >> 8) & 0xFF));
                        eepromWrite((EEPROM_TUNE_CHANNEL_ADDRESS + 1), (tunerChannel & 0xFF));
                    }
                }
            }
        } 

        delay_cycle(190);           
    }
}

void initSystem()
{
    // PC3 [OUT] : Serial data.
    // PC4 [OUT] : Latch data.
    // PC5 [OUT] : Serial clock.
    // PC6 [OUT] : Channel available in memory manager indicator (Preset channel).
    // PC7 [OUT] : Stereo indicator.
    PC_ODR = 0x00;
    PC_DDR = 0xF8;
    PC_CR1 = 0xF8;
    PC_CR2 = 0x00;  

    // PD1 [OUT] : Seven segment digit 1 driver.
    // PD2 [OUT] : Seven segment digit 2 driver.
    // PD3 [OUT] : Seven segment digit 3 driver.
    // PD4 [OUT] : Seven segment digit 4 driver. 
    // PD5 [IN]  : UP button.
    // PD6 [IN]  : DOWN button.
    PD_ODR = 0x00; 
    PD_DDR = 0x1E;
    PD_CR1 = 0x7E;
    PD_CR2 = 0x00;

    // PA1 [IN] : Volume UP button.
    // PA2 [IN] : Volume Down button.
    // PA3 [IN] : Memory.
    PA_DDR = 0x00;
    PA_CR1 = 0x0E;
    PA_CR2 = 0x00;

    // Initialize global variables.
    displayDecimal = CLEAR_SSD_DECIMAL;
    currentMode = mdFreq;
    modeResetCounter = 0;
    volumeLevel = 0;    
    lastCheckChannel = 0xFFFF;
}

void initDisplayTimer()
{
    // Set timer 2 prescaler to 256.
    TIM2_PSCR = 8;

    // Set ARRH to get ms interrupt.
    TIM2_ARRH = 0x00;
    TIM2_ARRL = 0x27;

    // Enable timer 2 interrupt.
    TIM2_IER |= TIM2_IER_UIE;
    TIM2_CR1 |= TIM2_CR1_CEN;
}

void setVolumeOnDisplay()
{
    // First two digits are always off in this mode.
    displayValue[0] = 0xFF;
    displayValue[1] = 0xFF;
    displayValue[2] = ((volumeLevel / 10) ? 1 : 0xFF);
    displayValue[3] = volumeLevel % 10;
}

unsigned short getSavedTunerChannel()
{
    return ((unsigned short)eepromRead(EEPROM_TUNE_CHANNEL_ADDRESS) << 8) | (eepromRead(EEPROM_TUNE_CHANNEL_ADDRESS + 1));
}

unsigned char memoryManager(unsigned char *stationNumber, unsigned short *channel)
{   
    unsigned char buttonState;
    unsigned short memAddr;
    unsigned short idleCounter;
    
    // Display "S" for memory manager station.
    displayDecimal = CLEAR_SSD_DECIMAL;
    displayValue[0] = 5;
    displayValue[1] = 0xFF;

    buttonState = (PD_IDR & 0x60) | (PA_IDR & 0x0E);
    idleCounter = 0;

    while(1)
    {
        // Display current station number.
        displayValue[2] = (((*stationNumber) == MAX_STATION_NUMBER) ? 1 : 0xFF);
        displayValue[3] = (*stationNumber) % 10;

        // Check for UP button event.
        if(((PD_IDR & 0x20) == 0x20) && ((buttonState & 0x20) == 0x00))
        {
            (*stationNumber) = ((*stationNumber) >= MAX_STATION_NUMBER) ? MIN_STATION_NUMBER : ((*stationNumber) + 1);
            idleCounter = 0;
        }

        // Check for DOWN button event.
        if(((PD_IDR & 0x40) == 0x40) && ((buttonState & 0x40) == 0x00))
        {
            (*stationNumber) = ((*stationNumber) == MIN_STATION_NUMBER) ? MAX_STATION_NUMBER : ((*stationNumber) - 1);
            idleCounter = 0;
        }

        // Check for STORE button event.
        if(((PA_IDR & 0x02) == 0x02) && ((buttonState & 0x02) == 0x00))
        {
            memAddr = EEPROM_MEM_MANAGER_BASE + ((*stationNumber) * 2);
            eepromWrite(memAddr, (((*channel) >> 8) & 0xFF));
            eepromWrite((memAddr + 1), ((*channel) & 0xFF));
            return 0;
        }

        // Check for RECALL button event.
        if(((PA_IDR & 0x04) == 0x04) && ((buttonState & 0x04) == 0x00))
        {
            memAddr = EEPROM_MEM_MANAGER_BASE + ((*stationNumber) * 2);
            (*channel) = ((unsigned short)eepromRead(memAddr) << 8) | (eepromRead(memAddr + 1));

            if((*channel) > 0x3FF)
            {
                // Tuner channel is invalid or first time load.
                (*channel) = 0;
            }

            delay_ms(100);
            return 1;
        }

        // Check for memory manager button event.
        if(((PA_IDR & 0x08) == 0x08) && ((buttonState & 0x08) == 0x00))
        {
            delay_ms(60);
            return 0;
        }

        // Check for memory manager idle timeout.
        if((++idleCounter) > MEM_MANAGER_IDLE_TIME)
        {
            // Memory manager idle timeout is reached.
            return 0;
        }

        buttonState = (PD_IDR & 0x60) | (PA_IDR & 0x0E);

        // Get current frequency and status data from the RDA receiver.
        getReceiverConfig(RECEIVER_READ_CONFIG_LEN);

        // Update stereo indicator.
        if(isStereoChannel())
        {
            // Stereo channel.
            PC_ODR |= 0x80;
        }
        else
        {
            // Mono channel.
            PC_ODR &= 0x7F;
        }

        delay_cycle(280); 
    }
}

void isPresetChannel(unsigned short *currentChannel)
{
    unsigned char scanPos = MAX_STATION_NUMBER;
    unsigned short memChannel;
    unsigned short memAddr;

    if((*currentChannel) != (lastCheckChannel))
    {
        // Current channel is different from the last checked channel.
        lastCheckChannel = (*currentChannel);

        while(scanPos > (MIN_STATION_NUMBER - 1))
        {
            // Extract saved frequency from the memory solt.
            memAddr = EEPROM_MEM_MANAGER_BASE + (scanPos * 2);
            memChannel = ((unsigned short)eepromRead(memAddr) << 8) | (eepromRead(memAddr + 1));
            
            // Check for identical channel data.
            if(memChannel == lastCheckChannel)
            {
                // Saved channel and current channel are identical.
                break;
            }

            // Move to next memory slot.
            scanPos--;
        }

        if(scanPos != 0)
        {
            // Loop is terminated in the middle, so given channel is available.
            PC_ODR |= 0x40;
        }
        else
        {
            // Given channel is not available in memory manager.
            PC_ODR &= 0xBF;
        }        
    }
}