#include <ESPemulation.h>

SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE1);

uint8_t SPI_TXpacket[4]; 
uint8_t SPI_RXpacket[4]; 

//void transfer(void *buf, size_t count)


// ROM Select
void setROMselect(uint8_t rom)
{
  if (rom < 4)
  {
    // set rom selection pins
  }
  else
  {
    // error condition here
  }

  return;
}

// ESP SPI Bus
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

// ESP Parallel BUS Functions
void clearBUS()
{
  delay(500);
  setSPIpacket(255,CONTROLBYTE_CLEAR,0);
  writeSPI();
  
  digitalWrite(CONTROL_Local,1);
  digitalWrite(CONTROL_Z80,1);

  digitalWrite(SBUS_OE_out,HIGH);  // disconnect S-Regs from BUS
}

uint8_t doBUSRead(uint16_t Address, uint8_t Control)
{
  setSPIpacket(255, Control, Address);
  writeSPI();
  // readSPI();
  clearBUS();
  return 0;
}

void doBUSWrite(uint8_t Data, uint16_t Address, uint8_t Control)
{
  setSPIpacket(Data, Control, Address);
  writeSPI();
  sendPULSE();
  clearBUS();
  return;
}




// ESP accessing Cache
uint8_t doCacheDataRead(uint16_t Address) //A16-0
{
  digitalWrite(CONTROL_Local,0);

  clearBUS();
  return 0;
}

uint8_t doCacheStatusRead(uint16_t Address) //A16-1
{
  digitalWrite(CONTROL_Local,0);

  clearBUS();
  return 0;
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) //A16-0
{
  digitalWrite(CONTROL_Local,0);
  
  doBUSWrite()

  clearBUS();
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) //A16-1
{
  digitalWrite(CONTROL_Local,0);

  clearBUS();
}


// ESP accessing the ROM/IOd
uint8_t doROMRead(uint16_t Address) //A16-0
{
  return 0;
}

uint8_t doIOdRead(uint16_t Address )//A16-1
{
  return 0;
}

void doROMWrite(uint8_t Data, uint16_t Address) //A16-0
{

}

void doIOdWrite(uint8_t Data, uint16_t Address) //A16-1
{

}


// ESP controlling the Z80
uint8_t doZ80IORead(uint16_t Address)
{
  return 0;
}

uint8_t doZ80MEMRead(uint16_t Address)
{
  return 0;
}

void doZ80IOWrite(uint8_t Data, uint16_t Address)
{

}

void doZ80MEMWrite(uint8_t Data, uint16_t Address)
{

}