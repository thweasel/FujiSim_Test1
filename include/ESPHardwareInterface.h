#include <ESPHardware_emulation.h>

/*
    IOd CONFIG Byte

    0 - CACHE select 0 [1]
    1 - CACHE select 1 [1]
    2 - CACHE select 2 [1]
    3 - CACHE select 3 [1]
    4 - CACHE data[0]/status[1]
    5 - WAIT_IO [0]
    6 - RESET Z80HARDLOCK [1]
    7 - SET Z80HARDLOCK [1]
*/

                        //76543210
#define IODCONFIG_STATUS B10010000  // READ a status byte  -- Start IO

// DATA IO address
#define IODCONFIG_CACHE0 B10000000
#define IODCONFIG_CACHE1 B10000010
#define IODCONFIG_CACHE2 B10000100
#define IODCONFIG_CACHE3 B10001000

#define IODCONFIG_IOEND  B01010000  // WRITE a status BYTE  -- End IO

// WAIT based addresses
#define IODCONFIG_WAIT   B00100000


void ESP_setup(void);

// ROM/IOd CHIP
uint8_t doCEROMRead(uint16_t Address, uint8_t ROMbank); //A16-0
void doCEROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank); //A16-0

uint8_t doIOdRead(uint16_t Address ); //A16-1
void doIOdWrite(uint8_t Data, uint16_t Address); //A16-1

// CACHE CHIP
uint8_t doCacheDataRead(uint16_t Address);//A16-0
void doCacheDataWrite(uint8_t Data, uint16_t Address); //A16-0

uint8_t doCacheStatusRead(uint16_t Address); //A16-1
void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1


// Z80 SYSTEM
uint8_t doZ80MEMRead(uint16_t Address);
void doZ80MEMWrite(uint8_t Data, uint16_t Address);


uint8_t doZ80IORead(uint16_t Address);
void doZ80IOWrite(uint8_t Data, uint16_t Address);

void enableROM(uint8_t ROMbank);
void disableROM(void);