#include <Arduino.h>
#include <SPI.h>

#define SBUS_STC_out 49
#define SBUS_OE_out 48
#define ESP_PULSE 47
#define TRIGGER_Local 12
#define TRIGGER_Z80 11
#define ESP_HARDLOCK 10
#define Z80_HARDLOCK 9
#define CONNECT_Z80 8

/*
Z80 control
0 - RD
1 - WR
2 - IORQ
3 - MEMRQ
4 - BUSRQ
5 - WAIT
6 - ROMCE
7 - NMI
*/ 
                       //76543210
#define CONTROL_CLEAR   B11111111
#define CONTROL_MEMRQRD B11110110
#define CONTROL_MEMRQWR B11110101
#define CONTROL_IORQRD  B11111010
#define CONTROL_IORQWR  B11111001

void doSBUSClear();

uint8_t doIORead(uint16_t Address);
void doIOWrite(uint8_t Data, uint16_t Address);

uint8_t doMEMRead(uint16_t Address);
void doMEMWrite(uint8_t Data, uint16_t Address);
