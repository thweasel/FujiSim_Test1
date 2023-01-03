#include <Z80Hardware_emulation.h>

void Z80Hardware_setup(void)
{
  // Z80 pin Config
  pinMode(Z80_BUSACK, OUTPUT);
  pinMode(Z80_RD, OUTPUT);
  pinMode(Z80_WR, OUTPUT);
  pinMode(Z80_IORQ, OUTPUT);
  pinMode(Z80_MEMRQ, OUTPUT);
  pinMode(Z80_WAIT, INPUT);
  pinMode(Z80_ROMCS, INPUT);
  pinMode(Z80_NMI, INPUT);
  // Z80 pin State
  digitalWrite(Z80_BUSACK, HIGH);
  digitalWrite(Z80_RD, HIGH);
  digitalWrite(Z80_WR, HIGH);
  digitalWrite(Z80_IORQ, HIGH);
  digitalWrite(Z80_MEMRQ, HIGH);
  digitalWrite(Z80_WAIT, HIGH);
  digitalWrite(Z80_ROMCS, HIGH);
  digitalWrite(Z80_NMI, HIGH);
  // Z80 Port Config
  Z80_DATA_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;
  // Z80 Port State
  Z80_DATA = 255;
  Z80_ADDR_L = 255;
  Z80_ADDR_H = 255;
  return;
}



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
    digitalWrite(Z80_WR, LOW);
    digitalWrite(Z80_WR, HIGH);
    
    //clear_Z80_DATA();
    return;
}

uint8_t read_Z80_DATA()
{
    Z80_DATA_DDR = PORT_DDR_INPUT;
    uint8_t data;

    digitalWrite(Z80_RD, LOW);
    data = Z80_DATA;
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
