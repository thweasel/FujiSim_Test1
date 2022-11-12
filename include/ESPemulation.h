#include <Arduino.h>
#include <SPI.h>

#define SBUS_STC_out 49
#define SBUS_OE_out 48
#define ESP_PULSE 47
#define CONTROL_Local 12
#define CONTROL_Z80 11
#define ESP_HARDLOCK 10
#define Z80_HARDLOCK 9
#define CONNECT_Z80 8

/*
Z80 control
0 - RD -- Z80_RD / Local_RD
1 - WR -- Z80_WR / Local_WD
2 - IORQ -- Z80_IORQ / Cache_Data/Status (0/1)
3 - MEMRQ -- Z80_MEMRQ / Local_MEMRQ
4 - BUSRQ -- Z80_BUSRQ
5 - WAIT -- Z80_WAIT
6 - ROMCE -- Z80_ROMCE
7 - NMI -- Z80_NMI
*/ 
                       //76543210
#define CONTROLBYTE_CLEAR   B11111111
// Z80 BUS
#define CONTROLBYTE_MEMRQ_RD B11110110
#define CONTROLBYTE_MEMRQ_WR B11110101
#define CONTROLBYTE_IORQ_RD  B11111010
#define CONTROLBYTE_IORQ_WR  B11111001
// Local BUS
#define CONTROLBYTE_CACHEDATA_RD  B11111010
#define CONTROLBYTE_CACHEDATA_WR  B11111001
#define CONTROLBYTE_CACHESTATUS_RD  B11111110
#define CONTROLBYTE_CACHESTATUS_WR  B11111101

void clearBUS();

uint8_t doIORead(uint16_t Address);
void doIOWrite(uint8_t Data, uint16_t Address);

uint8_t doMEMRead(uint16_t Address);
void doMEMWrite(uint8_t Data, uint16_t Address);
