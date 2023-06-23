#include <ESPHardware_emulation.h>

#define RUNSLOW true
#define SLOWTIME 200

SPISettings mySpiSettings_write (1000000,MSBFIRST,SPI_MODE0);
SPISettings mySpiSettings_read (1000000,MSBFIRST,SPI_MODE0);

//SPISettings mySpiSettings_write (500000,MSBFIRST,SPI_MODE1);
//SPISettings mySpiSettings_read (500000,MSBFIRST,SPI_MODE3);

uint8_t SPIpacketTX[4]; 
uint8_t SPIpacketRX[4];

// SPI PACKET BYTES
//  SPIpacketTX[3] = Data;
//  SPIpacketTX[2] = Control;
//  SPIpacketTX[1] = loAddr;
//  SPIpacketTX[0] = hiAddr;

//void transfer(void *buf, size_t count)

void ESPHardware_setup(void)
{
  // ESP pin Config
  pinMode(ESPout_STC, OUTPUT);
  pinMode(ESPout_OE, OUTPUT);
  pinMode(ESP_PULSE, OUTPUT);
  pinMode(ESPin_PL, OUTPUT);
  pinMode(ESP_ROMSELECT0, OUTPUT);
  pinMode(ESP_ROMSELECT1, OUTPUT);
  pinMode(ESP_espHARDLOCK, OUTPUT);
  pinMode(ESP_z80HARDLOCK, INPUT);
  pinMode(CONNECT_Z80, OUTPUT);

  // ESP pin State
  digitalWrite(ESPout_STC, HIGH);
  digitalWrite(ESPout_OE, HIGH);
  digitalWrite(ESP_PULSE, HIGH);
  digitalWrite(ESPin_PL, HIGH);
  digitalWrite(ESP_ROMSELECT0, LOW);
  digitalWrite(ESP_ROMSELECT1, LOW);
  digitalWrite(ESP_espHARDLOCK, HIGH); // Lock set LOW
  digitalWrite(ESP_z80HARDLOCK, HIGH); // Lock set LOW
  digitalWrite(CONNECT_Z80, HIGH);  // Connect on LOW (may be buffer with NOT?)
  return;
}


void clearSPIpacketRX (void)
{
  SPIpacketTX[3] = 0xFF;
  SPIpacketTX[2] = 0xFF;
  SPIpacketTX[1] = 0xFF;
  SPIpacketTX[0] = 0xFF;
}


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

uint8_t * setSPIpacketRX (uint8_t Data, uint8_t Control, uint16_t Address)
{

  uint8_t loAddr = (uint8_t)(Address);
  uint8_t hiAddr = (uint8_t)(Address >> 8);

  SPIpacketRX[3] = Data;
  SPIpacketRX[2] = Control;
  SPIpacketRX[1] = loAddr;
  SPIpacketRX[0] = hiAddr;

  return SPIpacketTX;
}

// ROM Select
void selectROMbank(uint8_t rom)
{
  switch (rom)
  {
  case 0:
    digitalWrite(ESP_ROMSELECT0,0);
    digitalWrite(ESP_ROMSELECT1,0);
    break;
  case 1:
    digitalWrite(ESP_ROMSELECT0,1);
    digitalWrite(ESP_ROMSELECT1,0);
    break;
  case 2:
    digitalWrite(ESP_ROMSELECT0,0);
    digitalWrite(ESP_ROMSELECT1,1);
    break;
  case 3:
    digitalWrite(ESP_ROMSELECT0,1);
    digitalWrite(ESP_ROMSELECT1,1);
    break;
  default:
    digitalWrite(ESP_ROMSELECT0,0);
    digitalWrite(ESP_ROMSELECT1,0);
    break;
  }

  return;
}

// Some actions need a clock pulse to trigger, use this.
void sendPULSE(void)
{
  digitalWrite(ESP_PULSE,ENABLE_LOW);
  delay(10);
  if(RUNSLOW) delay(SLOWTIME);
  digitalWrite(ESP_PULSE,DISABLE_HIGH);
  return;
}

bool setESPHardlock(void)
{  // This should check for the Z80 lock, then block or return false
  digitalWrite(ESP_espHARDLOCK,ENABLE_HIGH);
  
  while(1 == digitalRead(ESP_z80HARDLOCK))
  {
    delay(1); // wait -- block
    Serial.print("\n!! Z80_Hardlock !!\n");
  }
  
  return true;
}

bool resetESPHardlock(void)
{
  digitalWrite(ESP_espHARDLOCK,DISABLE_LOW);
  return true;
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
  digitalWrite(ESPout_STC, HIGH); // Move shift to storage
  digitalWrite(ESPout_STC, LOW);
  
  // Enable the Output from the 595s
  digitalWrite(ESPout_OE, ENABLE_LOW);

  delay(10);
  
  if(RUNSLOW) delay(SLOWTIME);

  return;
}

// SPI Read the ESPin
void readSPI(void)
{ 
  // INPUT 74xx165 Shift Registers (x4)

  // LATCH data into the Shift Regs (pulse)
  digitalWrite(ESPin_PL, LOW);  // Parallel Load
  delay(2);
  // Unload the Shift Reg to RXPacket
  digitalWrite(ESPin_PL, HIGH); // Serial Shift
  
  SPI.beginTransaction(mySpiSettings_read);
  SPI.transfer(SPIpacketRX,4);  
  SPI.endTransaction();

  delay(10);
  if(RUNSLOW) delay(SLOWTIME);

  digitalWrite(ESPout_OE,DISABLE_HIGH);  // Remove Control & Address  
  
}

// ESP Parallel BUS Functions

bool doBUSRQ(void)
{
  setSPIpacketTX(0xFF,CONTROLBYTE_BUSRQ_ENABLE,0xFFFF);
  writeSPI();
  // WAIT HERE FOR THE BUSACK signal
  delay(10);
  if(RUNSLOW) delay(SLOWTIME);
  return true;
}

bool resetBUSRQ(void)
{
  setSPIpacketTX(0xFF,CONTROLBYTE_BUSRQ_DISABLE,0xFFFF);
  writeSPI();
  // WAIT HERE FOR THE BUSACK signal
  delay(10);
  if(RUNSLOW) delay(SLOWTIME);
  return true;
}

void clearBUS()
{ 
  resetBUSRQ(); 
  digitalWrite(ESPout_OE,HIGH);  // disconnect S-Regs from BUS
  if(RUNSLOW) delay(SLOWTIME);

}

uint8_t * doBUSRead(uint16_t Address, uint8_t Control)
{
  setSPIpacketTX(0x0, Control, Address);
  writeSPI(); // Set Control and Address lines
  
  //clearSPIpacketRX();
  readSPI();
  
  //clearBUS();
  return SPIpacketRX;
}

void doBUSWrite(uint8_t Data, uint16_t Address, uint8_t Control)
{
  setSPIpacketTX(Data, Control, Address);
  writeSPI();
  sendPULSE();
  //clearBUS();
  return;
}


void setROMCS(void)
{
  setSPIpacketTX(0xff,CONTROLBYTE_ROMCS_ENABLE,0xFFFF);
  writeSPI();
  if(RUNSLOW) delay(SLOWTIME);

}

void resetROMCS(void)
{
  setSPIpacketTX(0xff,CONTROLBYTE_ROMCS_DISABLE,0xFFFF);
  writeSPI();
  if(RUNSLOW) delay(SLOWTIME);

}



