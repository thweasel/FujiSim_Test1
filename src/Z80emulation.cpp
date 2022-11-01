#include <Z80emulation.h>


void Z80_WR_Pulse()
{
  digitalWrite(Z80_WR,LOW);
  //_delay_us;
  delay(1000);
  digitalWrite(Z80_WR,HIGH);
}

void Z80_RD_Pulse()
{
  digitalWrite(Z80_RD,LOW);
  //_delay_us;
  delay(1000);
  digitalWrite(Z80_RD,HIGH);
}


void set_Z80_ADDR(uint16_t Address)
{
  Z80_ADDR_L = (uint8_t) Address;
  Z80_ADDR_H = (uint8_t) (Address >> 8);

}

void clear_Z80_ADDR()
{ // Default float address High?
  Z80_ADDR_L = 255;
  Z80_ADDR_H = 255;
}

void set_Z80_Data(uint8_t Data)
{
  Z80_DATA = Data;
}

void clear_Z80_DATA()
{ // Default float data High?
  Z80_DATA = 255;
}

void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
  digitalWrite(Z80_IORQ,LOW);
  set_Z80_ADDR(Address);
  set_Z80_Data(Data);
  Z80_WR_Pulse();
  digitalWrite(Z80_IORQ,HIGH);
  clear_Z80_ADDR();
  clear_Z80_DATA();
  digitalWrite(Z80_IORQ,HIGH);
  delay(1000);

}