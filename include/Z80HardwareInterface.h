#include <Arduino.h>
#include <Z80Hardware_emulation.h>

void Z80_setup(void);

void do_Z80_IOWR(uint16_t Address, uint8_t Data);
uint8_t do_Z80_IORD(uint16_t Address);

void do_Z80_MEMWR(uint16_t Address, uint8_t Data);
uint8_t do_Z80_MEMRD(uint16_t Address);