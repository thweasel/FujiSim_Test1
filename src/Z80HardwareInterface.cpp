#include <Z80HardwareInterface.h>


void Z80_setup(void)
{
    Z80Hardware_setup();
}

void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
    sendZ80_ADDR(Address);
    sendZ80_IORQ();    
    sendZ80_DATA(Data);
    Z80_IDLE();

    return;
}

uint8_t do_Z80_IORD(uint16_t Address)
{
    uint8_t data = 0;

    sendZ80_ADDR(Address);
    sendZ80_IORQ();        
    data = fetchZ80_DATA();
    Z80_IDLE();
    
    return data;
}

void do_Z80_MEMWR(uint16_t Address, uint8_t Data)
{
    sendZ80_ADDR(Address);
    sendZ80_MEMRQ();
    sendZ80_DATA(Data);
    Z80_IDLE();

    return;
}

uint8_t do_Z80_MEMRD(uint16_t Address)
{
    uint8_t data = 0;
    
    sendZ80_ADDR(Address);
    sendZ80_MEMRQ();
    data = fetchZ80_DATA();
    Z80_IDLE();  

    return data;
}