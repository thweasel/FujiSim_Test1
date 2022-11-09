#include <ESPemulation.h>

SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE1);

uint8_t SPIpacket[4]; 

//void transfer(void *buf, size_t count)

uint8_t * setSPIpacket (uint8_t Data, uint8_t Control, uint16_t Address)
{

  uint8_t loAddr = (uint8_t)(Address);
  uint8_t hiAddr = (uint8_t)(Address >> 8);

  SPIpacket[3] = Data;
  SPIpacket[2] = Control;
  SPIpacket[1] = loAddr;
  SPIpacket[0] = hiAddr;

  return SPIpacket;
}


void writeSPI(void)
{
  // Load the Shift Regs
  SPI.beginTransaction(mySpiSettings);
  SPI.transfer(SPIpacket,4);  
  SPI.endTransaction();

  // LATCH the Shift Regs to Storage (pulse)
  digitalWrite(SBUS_STC_out, LOW);
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_STC_out, LOW);
  
  // Enable the Output from the 595s
  digitalWrite(SBUS_OE_out,LOW);
  delay(500);
  digitalWrite(SBUS_OE_out,HIGH);
  return;
}

void doSBUSClear()
{
  setSPIpacket(255,CONTROL_CLEAR,0);
  writeSPI();
}

uint8_t doIORead(uint16_t Address)
{
  setSPIpacket(255, CONTROL_IORQRD, Address);
  writeSPI();
  return 0;
}

void doIOWrite(uint8_t Data, uint16_t Address)
{
  setSPIpacket(Data, CONTROL_IORQWR, Address);
  writeSPI();
}

uint8_t doMEMRead(uint16_t Address)
{
  setSPIpacket(255, CONTROL_MEMRQRD, Address);
  writeSPI();
  return 0;
}

void doMEMWrite(uint8_t Data, uint16_t Address)
{
  setSPIpacket(Data, CONTROL_MEMRQWR, Address);
  writeSPI();
}