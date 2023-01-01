#include <Arduino.h>
#include <SPI.h>

#define SBUS_STC_out 49
#define SBUS_OE_out 48
#define ESP_PULSE 47
#define ESPin_PL 46
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
#define CONTROLBYTE_CLEAR           B11111111
// Z80 BUS
#define CONTROLBYTE_CEROMRQ_RD      B10110110
#define CONTROLBYTE_CEROMRQ_WR      B10110101
#define CONTROLBYTE_MEMRQ_RD        B11110110
#define CONTROLBYTE_MEMRQ_WR        B11110101
#define CONTROLBYTE_IORQ_RD         B11111010
#define CONTROLBYTE_IORQ_WR         B11111001

// Local BUS
#define CONTROLBYTE_CACHEDATA_RD    B11111010
#define CONTROLBYTE_CACHEDATA_WR    B11111001
#define CONTROLBYTE_CACHESTATUS_RD  B11111110
#define CONTROLBYTE_CACHESTATUS_WR  B11111101


// Signals
#define DISABLE_HIGH HIGH
#define ENABLE_LOW LOW

#define DISABLE_LOW LOW
#define ENABLE_HIGH HIGH



uint8_t * setSPIpacketTX (uint8_t Data, uint8_t Control, uint16_t Address);

void selectROMbank(uint8_t rom);

void sendPULSE(void);

void writeSPI(void);
void readSPI(void);

void enableLocalControlBus();
void disableLocalControlBus();

void enableZ80ControlBus();
void disableZ80ControlBus();

void clearBUS();

uint8_t * doBUSRead(uint16_t Address, uint8_t Control);

void doBUSWrite(uint8_t Data, uint16_t Address, uint8_t Control);