#include <ESPemulation.h>

SPISettings mySpiSettings_write (40000,MSBFIRST,SPI_MODE0);
SPISettings mySpiSettings_read (40000,MSBFIRST,SPI_MODE0);

//SPISettings mySpiSettings_write (500000,MSBFIRST,SPI_MODE1);
//SPISettings mySpiSettings_read (500000,MSBFIRST,SPI_MODE3);

uint8_t SPIpacketTX[4]; 
uint8_t SPIpacketRX[4];

//void transfer(void *buf, size_t count)




// ESP SPI Bus and Pin controls
uint8_t * setSPIpacketTX (uint8_t Data, uint8_t Control, uint16_t Address)
{

  uint8_t loAddr = (uint8_t)(Address);
  uint8_t hiAddr = (uint8_t)(Address >> 8);

  SPIpacketTX[3] = Data;
  SPIpacketTX[2] = Control;
  SPIpacketTX[1] = loAddr;
  SPIpacketTX[0] = hiAddr;

  return SPIpacketTX;
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
  digitalWrite(ESP_PULSE,ENABLE_LOW);
  digitalWrite(ESP_PULSE,DISABLE_HIGH);
  return;
}



// SPI Write ESPout
void writeSPI(void)
{ 
  // OUTPUT 74XX595 Shift registers (x4)
  
  // Load TXPacket into the Shift Regs
  SPI.beginTransaction(mySpiSettings_write);
  SPI.transfer(SPIpacketTX,4);  
  SPI.endTransaction();

  // LATCH the Shift Regs to Storage (pulse)
  digitalWrite(SBUS_STC_out, HIGH); // Move shift to storage
  digitalWrite(SBUS_STC_out, LOW);
  
  // Enable the Output from the 595s
  digitalWrite(SBUS_OE_out, ENABLE_LOW);
  return;
}

// SPI Read the ESPin
void readSPI(void)
{ 
  // INPUT 74xx165 Shift Registers (x4)

  // LATCH data into the Shift Regs (pulse)
  digitalWrite(ESPin_PL, LOW);  // Parallel Load
  
  // Unload the Shift Reg to RXPacket
  digitalWrite(ESPin_PL, HIGH); // Serial Shift
  
  SPI.beginTransaction(mySpiSettings_read);
  SPI.transfer(SPIpacketRX,4);  
  SPI.endTransaction();

  digitalWrite(SBUS_OE_out,DISABLE_HIGH);  // Remove Control & Address  
}


// ESP Parallel BUS Functions
void enableLocalControlBus()
{
  digitalWrite(CONTROL_Local,ENABLE_LOW);
}

void disableLocalControlBus()
{
  digitalWrite(CONTROL_Local,DISABLE_HIGH);
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
  disableLocalControlBus();
  disableZ80ControlBus();
  
  digitalWrite(SBUS_OE_out,HIGH);  // disconnect S-Regs from BUS
}

uint8_t * doBUSRead(uint16_t Address, uint8_t Control)
{

  setSPIpacketTX(0x0, Control, Address);
  writeSPI(); // Set Control and Address lines
  
  readSPI();
  clearBUS();
  return SPIpacketRX;
}

void doBUSWrite(uint8_t Data, uint16_t Address, uint8_t Control)
{
  setSPIpacketTX(Data, Control, Address);
  writeSPI();
  sendPULSE();
  clearBUS();
  return;
}



// ESP accessing Cache chip
uint8_t * doCacheDataRead(uint16_t Address) //A16-0
{
  enableLocalControlBus();
  doBUSRead(Address,CONTROLBYTE_CACHEDATA_RD);

  clearBUS();
  return SPIpacketRX;
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) //A16-0
{
  enableLocalControlBus();
  
  doBUSWrite(Data,Address,CONTROLBYTE_CACHEDATA_WR);

  clearBUS();
}

uint8_t * doCacheStatusRead(uint16_t Address) //A16-1
{
  enableLocalControlBus();
  doBUSRead(Address,CONTROLBYTE_CACHESTATUS_RD);

  clearBUS();

  digitalWrite(CONTROL_Local,HIGH);
  return SPIpacketRX;
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) //A16-1
{
  enableLocalControlBus();

  doBUSWrite(Data,Address,CONTROLBYTE_CACHESTATUS_WR);

  clearBUS();

  digitalWrite(CONTROL_Local,HIGH);

}


// ESP accessing the ROM/IOd chip VIA Z80 Bus
// Uses these to read/write ROMs into the IOd chip
uint8_t * doCEROMRead(uint16_t Address, uint8_t ROMbank) //A16-0
{
  enableZ80ControlBus();
  selectROMbank(ROMbank);
  // Z80 CE

  doBUSRead(Address,CONTROLBYTE_CEROMRQ_RD);
  clearBUS();

  return SPIpacketRX;
}

void doCEROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank) //A16-0
{
  enableZ80ControlBus();
  selectROMbank(ROMbank);

  doBUSWrite(Data,Address,CONTROLBYTE_CEROMRQ_WR);

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

