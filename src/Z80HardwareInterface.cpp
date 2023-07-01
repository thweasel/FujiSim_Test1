#include <Z80HardwareInterface.h>


void Z80_setup(void)
{
    Z80Hardware_setup();
}

void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
    pinMode(Z80_IORQ,OUTPUT);
    digitalWrite(Z80_IORQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_DATA(Data);

    Z80_IDLE();

    return;
}

uint8_t do_Z80_IORD(uint16_t Address)
{
    uint8_t data = 0;

    pinMode(Z80_IORQ,OUTPUT);
    digitalWrite(Z80_IORQ, LOW);
    

    set_Z80_ADDR(Address);
    data = read_Z80_DATA();

    Z80_IDLE();

    return data;
}

void do_Z80_MEMWR(uint16_t Address, uint8_t Data)
{
    pinMode(Z80_MEMRQ,OUTPUT);
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_DATA(Data);

    Z80_IDLE();

    return;
}

uint8_t do_Z80_MEMRD(uint16_t Address)
{
    uint8_t data = 0;
    
    pinMode(Z80_MEMRQ,OUTPUT);
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);

    data = read_Z80_DATA();

    Z80_IDLE();    

    return data;
}