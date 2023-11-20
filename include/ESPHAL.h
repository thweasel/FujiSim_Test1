// DO NOT MAKE THE HAL AVAILABLE HERE
#include <Arduino.h>

/*
Z80 control
0 - RD -- Z80_RD / Local_RD
1 - WR -- Z80_WR / Local_WD
2 - IORQ -- Z80_IORQ / [Local] Cache_Data/Status (0/1)
3 - MEMRQ -- Z80_MEMRQ / Local_MEMRQ
4 - BUSRQ -- Z80_BUSRQ
5 - WAIT -- Z80_WAIT
6 - ROMCE -- Z80_ROMCE // HostROM ENABLED LOW, DISABLE HIGHn ( ZX spectrum )
7 - NMI -- Z80_NMI
                    7     6        5      4       3       2      1     0
// Control OUTPUT [ NMI | ROMCE  | WAIT | BUSRQ | MEMRQ | IORQ | WR  | RD ]
// Control INPUT  [ NMI | BUSACK | WAIT | BUSRQ | MEMRQ | IORQ | WR  | RD ]

*/ 

#define CONTROL_MASK_RD     B00000001
#define CONTROL_MASK_WR     B00000010
#define CONTROL_MASK_IORQ   B00000100
#define CONTROL_MASK_MEMRQ  B00001000

#define CONTROL_MASK_BUSRQ  B00010000
#define CONTROL_MASK_WAIT   B00100000
#define CONTROL_MASK_BUSACK B01000000
#define CONTROL_MASK_ROMCE  B01000000
#define CONTROL_MASK_NMI    B10000000


                                //   76543210
#define CONTROLBYTE_CLEAR           B11111111

// Z80 NATIVE BUS OPERATIONS
// Memory access
#define CONTROLBYTE_MEMRQ_RD        B11100110
#define CONTROLBYTE_MEMRQ_WR        B11100101
// IO access
#define CONTROLBYTE_IORQ_RD         B11101010
#define CONTROLBYTE_IORQ_WR         B11101001

// Z80 FUJI OPERATIONS
// RIO-ROM (Z80 or ESP under BUSRQ)
#define CONTROLBYTE_CEROMRQ_RD      B10100110
#define CONTROLBYTE_CEROMRQ_WR      B10100101

// RIO-IODconfig (ESP under BUSRQ ONLY)
#define CONTROLBYTE_RIOCONFIG_RD    B11101110
#define CONTROLBYTE_RIOCONFIG_WR    B11101101


// CONTROL BYTE MASKS
#define CONTROLBYTE_BUSRQ_ENABLE    B11101111
#define CONTROLBYTE_BUSRQ_DISABLE   B11111111

#define CONTROLBYTE_HOSTROM_ENABLE  B11111111
#define CONTROLBYTE_HOSTROM_DISABLE B10111111

//                      7     6        5      4       3       2      1     0
// Control SPI OUTPUT [ NMI | ROMCE  | WAIT | BUSRQ | MEMRQ | IORQ | WR  | RD ]

// Local BUS                        
// CACHE data access - IORQ == A16   76543210
#define CONTROLBYTE_CACHEDATA_RD    B10110010
#define CONTROLBYTE_CACHEDATA_WR    B10110001
// CACHE status access - IORQ == A16 76543210
#define CONTROLBYTE_CACHESTATUS_RD  B10110110
#define CONTROLBYTE_CACHESTATUS_WR  B10110101


// Signals
//#define DISABLE_HIGH HIGH
//#define ENABLE_LOW LOW

//#define DISABLE_LOW LOW
//#define ENABLE_HIGH HIGH

//
// INTERUPT CODES

#define INT_NONE                0
#define INT_Z80HARDLOCK_RESET   1
#define INT_BUSACK              2
#define INT_WAIT                3





//
// BASIC bus operations exposed to BUS interface

// void setBUSidle(void);  // NOT a valid bus function outside of the HAL

uint8_t doReadBUSData(uint16_t Address, uint8_t Control);

void doWriteBUSData(uint8_t Data, uint16_t Address, uint8_t Control);

void ESPHardware_setup(void);

uint8_t pollINT(void);

boolean getLocalBus(uint8_t waitus);
void releaseLocalBus(void);

bool getZ80BUS(uint8_t waitus);

// bool releaseZ80BUS(void); // Implicit when LocalBus is release (Z80BUSRQ goes high)



//
// RIO ROM
void enableRIO_ROMRW(uint8_t ROMbank);
void enableRIO_ROMRD(uint8_t ROMbank);
void disableRIO_ROM(void);


//
// RIO IOdevice
void enableRIO_IOdConfigAccess(void);
void disableRIO_IOdConfigAccess(void);
