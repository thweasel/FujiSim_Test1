#include <Z80emulation.h>

void clear_Z80_ADDR()
{ // Default float address High?
    Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;

    Z80_ADDR_L = 255;
    Z80_ADDR_H = 255;

    return;
}

void set_Z80_ADDR(uint16_t Address)
{
    Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;

    Z80_ADDR_L = (uint8_t)Address;
    Z80_ADDR_H = (uint8_t)(Address >> 8);

    return;
}

uint16_t get_Z80_ADDR()
{
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;

    uint16_t address;
    address = Z80_ADDR_H;
    address = address << 8;
    address = address | Z80_ADDR_L;

    clear_Z80_ADDR();
    return address;
}

void clear_Z80_DATA()
{ // Default float data High?
    Z80_DATA = 0;
    Z80_DATA = 255;
    Z80_DATA_DDR = PORT_DDR_OUTPUT;
    

    return;
}

void send_Z80_Data(uint8_t Data)
{
    //Z80_DATA_DDR = PORT_DDR_OUTPUT;
    Z80_DATA = Data;
    delay(500);
    digitalWrite(Z80_WR, LOW);
    //_delay_us;
    delay(500);
    digitalWrite(Z80_WR, HIGH);
    

    //clear_Z80_DATA();
    return;
}

uint8_t read_Z80_DATA()
{
    Z80_DATA_DDR = PORT_DDR_INPUT;
    uint8_t data;
    delay(500);
    digitalWrite(Z80_RD, LOW);
    //_delay_us;
    delay(500);
    data = Z80_DATA;
    //Z80_DATA = 0; // Clear the register

    digitalWrite(Z80_RD, HIGH);

    //clear_Z80_DATA();
    return data;
}

void Z80_IDLE()
{


    digitalWrite(Z80_RD, HIGH);
    digitalWrite(Z80_WR, HIGH);
    digitalWrite(Z80_IORQ, HIGH);
    digitalWrite(Z80_MEMRQ, HIGH);
    digitalWrite(Z80_WAIT, HIGH);
    digitalWrite(Z80_ROMCS, HIGH);
    digitalWrite(Z80_NMI, HIGH);

    clear_Z80_ADDR();
    clear_Z80_DATA();

    return;
}

void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
    digitalWrite(Z80_IORQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_Data(Data);

    Z80_IDLE();
    delay(1000);

    return;
}

uint8_t do_Z80_IORD(uint16_t Address)
{
    digitalWrite(Z80_IORQ, LOW);

    set_Z80_ADDR(Address);
    uint8_t data = read_Z80_DATA();

    Z80_IDLE();
    delay(1000);

    return data;
}

void do_Z80_MEMWR(uint16_t Address, uint8_t Data)
{
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);
    send_Z80_Data(Data);

    Z80_IDLE();
    delay(1000);

    return;
}

uint8_t do_Z80_MEMRD(uint16_t Address)
{
    digitalWrite(Z80_MEMRQ, LOW);

    set_Z80_ADDR(Address);
    uint8_t data = read_Z80_DATA();

    Z80_IDLE();
    delay(1000);

    return data;
}