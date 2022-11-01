#include <Arduino.h>

// Z80 Emulation
#define Z80_DATA PORTA
#define Z80_DATA_DDR DDRA

#define Z80_ADDR_L PORTF
#define Z80_ADDR_L_DDR DDRF

#define Z80_ADDR_H PORTK
#define Z80_ADDR_H_DDR DDRK

#define Z80_RD    21
#define Z80_WR    20
#define Z80_IORQ  19
#define Z80_MEMRQ 18
#define Z80_BUSRQ 17
#define Z80_WAIT  16
#define Z80_ROMCS 15
#define Z80_NMI   14



void Z80_WR_Pulse();
void Z80_RD_Pulse();
void set_Z80_ADDR(uint16_t Address);
void clear_Z80_ADDR();
void set_Z80_Data(uint8_t Data);
void clear_Z80_DATA();
void do_Z80_IOWR(uint16_t Address, uint8_t Data);