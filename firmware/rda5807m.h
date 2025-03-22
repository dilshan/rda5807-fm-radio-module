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

#ifndef FM_MICRO_RDA5807M_HEADER
#define FM_MICRO_RDA5807M_HEADER

#define RECEIVER_WRITE_CONFIG_LEN 12    // 02H - 08H
#define RECEIVER_READ_CONFIG_LEN  4     // 0AH - 0BH

#define MAX_RDA_OUTPUT_VOLUME   0x0F

#define INIT_RX_REG_0	0xD0	// DHIZ | DMUTE | MONO | BASS | RCLK_MODE | RCLK | SEEKUP | SEEK
#define INIT_RX_REG_1	0x07	// SKMODE | CLK_MODE | CLK_MODE | CLK_MODE | RDS_EN | NEW_METHOD | SOFT_RESET | ENABLE

#define INIT_RX_REG_2	0x00	// CHAN
#define INIT_RX_REG_3	0x03	// CHAN | CHAN | DIRECT_MODE | TUNE | BAND | BAND | SPACE | SPACE

#define INIT_RX_REG_4	0x04	// RSVD | STCIEN | RBDS | RDS_FIFO_EN | DE | RDS_FIFO_CLR | SOFTMUTE_EN | AFCD
#define INIT_RX_REG_5	0x00	// RSVD | I2S_ENABLE | GPIO3 | GPIO3 | GPIO2 | GPIO2 | GPIO1 | GPIO1

#define INIT_RX_REG_6	0x88	// INT _MODE | SEEK_MODE | SEEK_MODE | RSVD | SEEKTH | SEEKTH | SEEKTH | SEEKTH
#define INIT_RX_REG_7	0x8F	// LNA_PORT_SEL | LNA_PORT_SEL | LNA_ICSEL_BIT | LNA_ICSEL_BIT | VOLUME | VOLUME | VOLUME | VOLUME

#define INIT_RX_REG_8	0x00	// RSVD | OPEN_MODE | OPEN_MODE | SLAVE_MASTER | WS_LR | SCLK_I_EDGE | DATA_SIGNED | WS_I_EDGE
#define INIT_RX_REG_9	0x00	// I2S_SW_CNT | I2S_SW_CNT | I2S_SW_CNT | I2S_SW_CNT | SW_O_EDGE | SCLK_O_EDGE | L_DELY | R_DELY

#define INIT_RX_REG_10	0x42	// RSVD | TH_SOFRBLEND | TH_SOFRBLEND | TH_SOFRBLEND | TH_SOFRBLEND | TH_SOFRBLEND | 65M_50M MODE | RSVD
#define INIT_RX_REG_11	0x02	// SEEK_TH_OLD | SEEK_TH_OLD | SEEK_TH_OLD | SEEK_TH_OLD | SEEK_TH_OLD | SEEK_TH_OLD | SOFTBLEND_EN | FREQ_MODE

void initRDAFMReceiver(unsigned char volLvl, unsigned short channel);
void updateReceiverConfig(unsigned char length);
void seekChannel(unsigned char isSeekUp);
void setRDAVolume(unsigned char volLvl);
void setTunerFrequency(unsigned short channel);

void getReceiverConfig(unsigned char length);
void getTunerFrequency(unsigned char *freq);
void getTunerChannel(unsigned short *channel);
unsigned char isStereoChannel();
unsigned char isStation();

#endif /* FM_MICRO_RDA5807M_HEADER */