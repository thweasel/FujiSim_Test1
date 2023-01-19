#include <Arduino.h>
#include <SPI.h>

#define ESPout_STC 49
#define ESPout_OE 48
#define ESP_PULSE 47
#define ESPin_PL 46
#define ESP_ROMSELECT1 12
#define ESP_ROMSELECT0 11
#define ESP_espHARDLOCK 10
#define ESP_z80HARDLOCK 9
#define CONNECT_Z80 8
#define Z80_ROMCS 15

/*
Z80 control
0 - RD -- Z80_RD / Local_RD
1 - WR -- Z80_WR / Local_WD
2 - IORQ -- Z80_IORQ / [Local] Cache_Data/Status (0/1)
3 - MEMRQ -- Z80_MEMRQ / Local_MEMRQ
4 - BUSRQ -- Z80_BUSRQ
5 - WAIT -- Z80_WAIT
6 - ROMCE -- Z80_ROMCE
7 - NMI -- Z80_NMI
*/ 
                       //76543210
#define CONTROLBYTE_CLEAR           B11111111
// Z80 BUS
#define CONTROLBYTE_CEROMRQ_RD      B10100110
#define CONTROLBYTE_CEROMRQ_WR      B10100101
#define CONTROLBYTE_MEMRQ_RD        B11100110
#define CONTROLBYTE_MEMRQ_WR        B11100101
#define CONTROLBYTE_IORQ_RD         B11101110
#define CONTROLBYTE_IORQ_WR         B11101101
#define CONTROLBYTE_BUSRQ_ENABLE    B11101111
#define CONTROLBYTE_BUSRQ_DISABLE   B11111111
#define CONTROLBYTE_ROMCS_ENABLE    B10111111
#define CONTROLBYTE_ROMCS_DISABLE   B10111111


// Local BUS
#define CONTROLBYTE_CACHEDATA_RD    B11110010
#define CONTROLBYTE_CACHEDATA_WR    B11110001
#define CONTROLBYTE_CACHESTATUS_RD  B11110110
#define CONTROLBYTE_CACHESTATUS_WR  B11110101


// Signals
#define DISABLE_HIGH HIGH
#define ENABLE_LOW LOW

#define DISABLE_LOW LOW
#define ENABLE_HIGH HIGH



uint8_t * setSPIpacketTX (uint8_t Data, uint8_t Control, uint16_t Address);

void ESPHardware_setup(void);

void selectROMbank(uint8_t rom);

void sendPULSE(void);

bool setESPHardlock(void);
bool resetESPHardlock(void);

void writeSPI(void);
void readSPI(void);

void clearBUS();

bool doBUSRQ(void);

uint8_t * doBUSRead(uint16_t Address, uint8_t Control);

void doBUSWrite(uint8_t Data, uint16_t Address, uint8_t Control);

void setROMCS(void);
void resetROMCS(void);
