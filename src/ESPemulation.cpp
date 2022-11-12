#include <ESPemulation.h>

SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE1);

uint8_t SPI_TXpacket[4]; 
uint8_t SPI_RXpacket[4]; 

//void transfer(void *buf, size_t count)




// ESP SPI Bus and Pin controls
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

// ROM Select
void selectROMbank(uint8_t rom)
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

// Some actions need a clock pulse to trigger, use this.
void sendPULSE(void)
{
  digitalWrite(ESP_PULSE,LOW);
  delay(1);
  digitalWrite(ESP_PULSE,HIGH);
  return;
}

// SPI write for setting up the BUSout
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
void enableLocalControlBus()
{
  digitalWrite(CONTROL_Local,0);
}

void disableLocalControlBus()
{
  digitalWrite(CONTROL_Local,1);
}
void enableZ80ControlBus()
{
  digitalWrite(CONTROL_Z80,0);
}
void disableZ80ControlBus()
{
  digitalWrite(CONTROL_Z80,1);
}

void clearBUS()
{
  delay(500);

  setSPIpacket(255,CONTROLBYTE_CLEAR,0);
  writeSPI();
  
  disableLocalControlBus();
  disableZ80ControlBus();
  
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



// ESP accessing Cache chip
uint8_t doCacheDataRead(uint16_t Address) //A16-0
{
  enableLocalControlBus();
  doBUSRead(Address,CONTROLBYTE_CACHEDATA_RD);

  clearBUS();
  return 0;
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) //A16-0
{
  enableLocalControlBus();
  
  doBUSWrite(Data,Address,CONTROLBYTE_CACHEDATA_WR);

  clearBUS();
}

uint8_t doCacheStatusRead(uint16_t Address) //A16-1
{
  enableLocalControlBus();
  doBUSRead(Address,CONTROLBYTE_CACHESTATUS_RD);

  clearBUS();
  return 0;
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) //A16-1
{
  enableLocalControlBus();

  doBUSWrite(Data,Address,CONTROLBYTE_CACHESTATUS_WR);

  clearBUS();
}


// ESP accessing the ROM/IOd chip VIA Z80 Bus
// Uses these to read/write ROMs into the IOd chip
uint8_t doROMRead(uint16_t Address, uint8_t ROMbank) //A16-0
{
  enableZ80ControlBus();
  selectROMbank(ROMbank);

  doBUSRead(Address,CONTROLBYTE_MEMRQ_WR);

  clearBUS();
  return 0;
}

void doROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank) //A16-0
{
  enableZ80ControlBus();
  selectROMbank(ROMbank);

  doBUSWrite(Data,Address,CONTROLBYTE_MEMRQ_WR);

  clearBUS();
}

// These methods are for programming the IOd configuration bytes
uint8_t doIOdRead(uint16_t Address ) //A16-1
{
  enableZ80ControlBus();
  // connect Z80Data and ROM/IOdbus
  
  doBUSRead(Address,CONTROLBYTE_IORQ_RD);

  clearBUS();
  return 0;
}

void doIOdWrite(uint8_t Data, uint16_t Address) //A16-1
{
  enableZ80ControlBus();
  // connect Z80Data and ROM/IOdbus

  doBUSWrite(Data,Address,CONTROLBYTE_IORQ_WR);

  clearBUS();
}


// ESP controlling the system in place of the Z80
// Direct access to the host system memory
uint8_t doZ80MEMRead(uint16_t Address)
{
  enableZ80ControlBus();

  doBUSRead(Address,CONTROLBYTE_MEMRQ_RD);

  clearBUS();
  return 0;
}

void doZ80MEMWrite(uint8_t Data, uint16_t Address)
{
  enableZ80ControlBus();
  
  doBUSWrite(Data,Address,CONTROLBYTE_MEMRQ_WR);

  clearBUS();
}

// IO calls may be needed to access system memory bank registers etc
// Probably will not be much use for interacting with actual hardware (does not create true bus timings)
uint8_t doZ80IORead(uint16_t Address)
{
  enableZ80ControlBus();

  doBUSRead(Address,CONTROLBYTE_IORQ_RD);

  clearBUS();
  return 0;
}

void doZ80IOWrite(uint8_t Data, uint16_t Address)
{
  enableZ80ControlBus();

  doBUSWrite(Data,Address,CONTROLBYTE_IORQ_WR);

  clearBUS();
}

