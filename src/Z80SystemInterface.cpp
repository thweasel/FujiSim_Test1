#include "Z80SystemInterface.h"


void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
    digitalWrite(Z80_IORQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_Data(Data);

    Z80_IDLE();
    delay(250);

    return;
}

uint8_t do_Z80_IORD(uint16_t Address)
{
    digitalWrite(Z80_IORQ, LOW);

    set_Z80_ADDR(Address);
    uint8_t data = read_Z80_DATA();

    Z80_IDLE();
    delay(250);

    return data;
}

void do_Z80_MEMWR(uint16_t Address, uint8_t Data)
{
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_Data(Data);

    Z80_IDLE();
    delay(250);

    return;
}

uint8_t do_Z80_MEMRD(uint16_t Address)
{
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);
    uint8_t data = read_Z80_DATA();

    Z80_IDLE();
    delay(250);

    return data;
}