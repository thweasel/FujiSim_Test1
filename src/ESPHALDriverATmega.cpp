
#include <ESPHALDriverATmega.h>
#include "consoleDebug.h"

#define DEBUG_SPI false
#define DEBUG_SPI_FORMAT BIN // BIN / HEX /DEC



#define RUNSLOW false
#define SLOWTIME 100
#define SLOWPULSEDIV 5



//void transfer(void *buf, size_t count)

//
// PIN Map


// ARDUINO SPI PINS
// SPI SS   53
// SPI SCK  52
// SPI MOSI 51
// SPI MISO 50

#define ESP_SPI_INT_STC 49  // OUTPUT (pulse)     --SPI Latch Bus packet in shift registers for output
#define ESP_SPI_INT_OE 48   // OUTPUT (send/stop) --SPI Enable Output of an Address from shift registers
#define ESP_PULSE 47        // OUTPUT (pulse)     -- Generate a pulse for Writing
#define ESP_SPI_INT_PL 46   // OUTPUT (pulse)     -- SPI Load Host BUS state to shift registers


#define ESP_RIO_PROTECT 5   // OUTPUT (enable/disable)
#define ESP_RESET 6         // OUTPUT (pulse)
#define ESP_INT 3           // INPUT  [INTERRUPT]

#define ESP_WAIT_RESET 8    // OUTPUT (pulse)
#define Z80_HARDLOCK 9      // INPUT  (get)
#define ESP_HARDLOCK 10     // OUTPUT (set/release)
#define ESP_ROMSELECT0 11   // OUTPUT (select ROM)
#define ESP_ROMSELECT1 12   // OUTPUT (select ROM)





//
// BUS BusPacketBuffer

/* 
  BUS PACKET BYTES
    BusPacketBuffer[3] = Data;
    BusPacketBuffer[2] = Control;
    BusPacketBuffer[1] = loAddr;
    BusPacketBuffer[0] = hiAddr;
*/

uint8_t BusPacketBuffer[4]; 

void clearBusPacketBuffer (void)
{
  BusPacketBuffer[3] = 0xFF; // Data
  BusPacketBuffer[2] = 0xFF; // Control
  BusPacketBuffer[1] = 0xFF; // LoAddr
  BusPacketBuffer[0] = 0xFF; // HiAddr
}

uint8_t * setBusPacketBuffer (uint8_t Data, uint8_t Control, uint16_t Address)
{

  uint8_t loAddr = (uint8_t)(Address);
  uint8_t hiAddr = (uint8_t)(Address >> 8);

  BusPacketBuffer[3] = Data;
  BusPacketBuffer[2] = Control;
  BusPacketBuffer[1] = loAddr;
  BusPacketBuffer[0] = hiAddr;

  return BusPacketBuffer;
}



//
// SPI BUS hardware implementation



SPISettings mySpiSettings (10000,MSBFIRST,SPI_MODE0);

// WRITE method for the bus hardware
void writeSPI(void)
{ 
  // OUTPUT 74XX595 Shift registers (x4)

  if(DEBUG_SPI) 
  {
    consoleShowBusPacketBuffer("writeSPI",BusPacketBuffer,DEBUG_SPI_FORMAT);
  }

  // Load TXPacket into the Shift Regs
  SPI.beginTransaction(mySpiSettings);
  SPI.transfer(BusPacketBuffer,4);  
  SPI.endTransaction();

  // LATCH the Shift Regs to Storage (pulse)
  digitalWrite(ESP_SPI_INT_STC, HIGH); // Move shift to storage
  
  if(RUNSLOW) { delay(SLOWTIME); } // SLOW THE OUTPUT POINT
  
  digitalWrite(ESP_SPI_INT_STC, LOW);

  return;
}

// READ method for the bus hardware
void readSPI(void)
{ 
  // INPUT 74xx165 Shift Registers (x4)

  // LATCH data into the Shift Regs (pulse)
  digitalWrite(ESP_SPI_INT_PL, LOW);  // Parallel Load
  if(RUNSLOW) { delay(SLOWTIME); }  // SLOW THE INPUT POINT
  // Unload the Shift Reg to RXPacket
  digitalWrite(ESP_SPI_INT_PL, HIGH); // Serial Shift
  
  //clearBusPacketBuffer();
  SPI.beginTransaction(mySpiSettings);
  SPI.transfer(BusPacketBuffer,4);  
  SPI.endTransaction(); 
  
  
  
  // Needed if Shift registers are chained using !Q7
  //BusPacketBuffer[0] = ~BusPacketBuffer[0];
  //BusPacketBuffer[2] = ~BusPacketBuffer[2];

  if(DEBUG_SPI) 
  {
    consoleShowBusPacketBuffer("readSPI",BusPacketBuffer,DEBUG_SPI_FORMAT);
  }

  return; // results in BusPacketBuffer
}

// BUS signal output controls
void sendBusSignals(void)
{
  digitalWrite(ESP_SPI_INT_OE, LOW);
}

void stopBusSignals(void)
{
  digitalWrite(ESP_SPI_INT_OE, HIGH);
}



//
// Interface for Hardware Abstraction Layer


void Setup_ESPHALDriver(void)
{
  // INPUT PINS
  pinMode(Z80_HARDLOCK, INPUT);
  digitalWrite(Z80_HARDLOCK, LOW); // Tri-State

  // ESP pin Config
  pinMode(ESP_SPI_INT_STC, OUTPUT);
  pinMode(ESP_SPI_INT_OE, OUTPUT);
  pinMode(ESP_PULSE, OUTPUT);
  pinMode(ESP_SPI_INT_PL, OUTPUT);
  pinMode(ESP_RIO_PROTECT, OUTPUT);
  pinMode(ESP_ROMSELECT0, OUTPUT);
  pinMode(ESP_ROMSELECT1, OUTPUT);
  pinMode(ESP_HARDLOCK, OUTPUT);
  pinMode(ESP_RIO_PROTECT, OUTPUT);

  // OUTPUT States
  digitalWrite(ESP_SPI_INT_STC, HIGH);
  digitalWrite(ESP_SPI_INT_OE, HIGH);
  digitalWrite(ESP_PULSE, HIGH);
  digitalWrite(ESP_SPI_INT_PL, HIGH);
  digitalWrite(ESP_RIO_PROTECT, HIGH);  // WRITE ENABLE LOW
  digitalWrite(ESP_ROMSELECT0, LOW);
  digitalWrite(ESP_ROMSELECT1, LOW);
  digitalWrite(ESP_HARDLOCK, HIGH);     // START LOCKED
  digitalWrite(ESP_RIO_PROTECT, HIGH);  // Connect on LOW (may be buffer with NOT?)
 
  return;
}


void WriteDataBUSOperation(uint8_t Data, uint8_t Control, uint16_t Address)
{
  setBusPacketBuffer(Data,Control,Address);  
  writeSPI(); // Set Control and Address lines
  sendBusSignals();
  sendPulse();
  return;
}

uint8_t ReadDataBUSOperation(uint8_t Control, uint16_t Address)
{ 
  setBusPacketBuffer(0x00,Control,Address);  
  writeSPI(); // Set Control and Address lines
  sendBusSignals();
  readSPI();
  return BusPacketBuffer[3]; // Data byte
}

uint8_t * getBUSstate(void) {
  readSPI();
  return BusPacketBuffer;
}


//
// RIO Hardware ROM/CACHE

void enableRIOProtection(void)
{
  digitalWrite(ESP_RIO_PROTECT, HIGH);
  return;
}

void disableRIOProtection(void)
{
  digitalWrite(ESP_RIO_PROTECT, LOW);
  return;
}

void setROMbank(uint8_t rom)
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


//
// OUTPUT send (Low/High)

void sendPulse(void)
{
  // Some actions need a low clock pulse to trigger
  // This pin should idel high
  digitalWrite(ESP_PULSE,LOW);
  if(RUNSLOW) { delay(SLOWTIME/SLOWPULSEDIV); }
  digitalWrite(ESP_PULSE,HIGH);
  return;
}
void sendWAITreset(void)
{
  digitalWrite(ESP_WAIT_RESET,LOW);
  if(RUNSLOW) { delay(SLOWTIME/SLOWPULSEDIV); }
  digitalWrite(ESP_WAIT_RESET,HIGH);
  return;
}

void sendRESET(void)
{
  digitalWrite(ESP_RESET,LOW);
  if(RUNSLOW) { delay(SLOWTIME/SLOWPULSEDIV); }
  delay(1); // Extend the delay as needed for clean reset of the host
  digitalWrite(ESP_RESET,HIGH);
  return;
}

//
// OUPUT set/release 

void setESPHardlock(void)       { digitalWrite(ESP_HARDLOCK,HIGH);   }
void clearESPHardlock(void)     { digitalWrite(ESP_HARDLOCK,LOW);    }


//
// INPUT get

boolean getZ80HardlockState(void) 
{ 
  if (digitalRead(Z80_HARDLOCK) == 1)
  {
    Serial.println("Z80 has lock");
    return true;
  }
  return false; 
}
boolean getINT(void)              { return !digitalRead(ESP_INT); }

