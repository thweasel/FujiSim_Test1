#include <ESPHardware_emulation.h>

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