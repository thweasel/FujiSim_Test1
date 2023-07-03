#include <Arduino.h>

// Bus commands
void sendZ80_IORQ(void);
void sendZ80_MEMRQ(void);

// Address bus operations
void sendZ80_ADDR(uint16_t Address);
uint16_t fetchZ80_ADDR();

// Data bus operations
void sendZ80_DATA(uint8_t Data);
uint8_t fetchZ80_DATA();

// Default state
void Z80_IDLE();

// CPU service routines
void serviceBUSREQ (void);

void Z80Hardware_setup(void);