#include <ESPemulation.h>

SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE1);

uint8_t SPI_TXpacket[4]; 
uint8_t SPI_RXpacket[4]; 

//void transfer(void *buf, size_t count)

uint8_t * setSPIpacket (uint8_t Data, uint8_t Control, uint16_t Address)
{

  uint8_t loAddr = (uint8_t)(Address);
  uint8_t hiAddr = (uint8_t)(Address >> 8);

  SPI_TXpacket[3] = Data;
  SPI_TXpacket[2] = Control;
  SPI_TXpacket[1] = loAddr;
  SPI_TXpacket[0] = hiAddr;

  return SPI_TXpacket;
}

void sendPULSE(void)
{
  digitalWrite(ESP_PULSE,LOW);
  delay(1);
  digitalWrite(ESP_PULSE,HIGH);
  return;
}

void writeSPI(void)
{
  // Load the Shift Regs
  SPI.beginTransaction(mySpiSettings);
  SPI.transfer(SPI_TXpacket,4);  
  SPI.endTransaction();

  // LATCH the Shift Regs to Storage (pulse)
  digitalWrite(SBUS_STC_out, LOW);
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_STC_out, LOW);
  
  // Enable the Output from the 595s
  digitalWrite(SBUS_OE_out,LOW);
  return;
}

void doSBUSClear()
{
  delay(500);
  setSPIpacket(255,CONTROL_CLEAR,0);
  writeSPI();
  digitalWrite(SBUS_OE_out,HIGH);  // disconnect S-Regs from BUS
}

uint8_t doIORead(uint16_t Address)
{
  setSPIpacket(255, CONTROL_IORQRD, Address);
  writeSPI();
  doSBUSClear();
  return 0;
}

uint8_t doMEMRead(uint16_t Address)
{
  setSPIpacket(255, CONTROL_MEMRQRD, Address);
  writeSPI();
  doSBUSClear();
  return 0;
}


void doIOWrite(uint8_t Data, uint16_t Address)
{
  setSPIpacket(Data, CONTROL_IORQWR, Address);
  writeSPI();
  sendPULSE();
  doSBUSClear();
  return;
}

void doMEMWrite(uint8_t Data, uint16_t Address)
{
  setSPIpacket(Data, CONTROL_MEMRQWR, Address);
  writeSPI();
  sendPULSE();
  doSBUSClear();
  return;
}