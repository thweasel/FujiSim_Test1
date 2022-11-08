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
  SPIpacket[1] = hiAddr;
  SPIpacket[0] = loAddr;

  return SPIpacket;
}

void clear_ESP_SBUS_out()
{

  setSPIpacket(255,255,0);

  // Load the Shift Regs
  SPI.beginTransaction(mySpiSettings);
  //SPI.transfer(255); // Addr HI
  //SPI.transfer(255); // Addr LO
  //SPI.transfer(255); // Z80 Control
  //SPI.transfer(255); // Local Control
  //SPI.transfer(255); // DATA
  
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
}

