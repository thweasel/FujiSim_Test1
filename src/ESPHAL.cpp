#include "ESPHALDriverATmega.h"
#include "ESPBUSInterface.h"
#include "ESPHAL.h"
#include "consoleDebug.h"

#define DEBUG_BUSRQ_BUSACK false

// Should the HAL track the last bus state written or read
//uint8_t dataRegister;
//uint8_t controlRegister;
//uint16_t addressRegister;

uint8_t controlByte;  // Control bus state

//
// BASIC bus operations exposed to BUS interface

void setBUSidle(void)
{
  // Default IDLE state for the system
  stopBusSignals();
  doWriteBUSData(0xff,0xffff,0xff);  // set all shift register outputs high
  return;
}

uint8_t doReadBUSData(uint16_t Address, uint8_t Control)
{
  uint8_t result;
  result = ReadDataBUSOperation(Control, Address);
  return result;
}

void doWriteBUSData(uint8_t Data, uint16_t Address, uint8_t Control)
{
  WriteDataBUSOperation(Data,Control,Address);
  return;
}



uint8_t pollINT(void) {

  // Control INPUT  [ RD | WE | IORQ | MEMRQ | BUSRQ | WAIT | BUSACK | NMI ]
  uint8_t intCode = 0;
  if( getINT() )
  {
    uint8_t *busPacket = getBUSstate(); // BusPacket [2] Control byte
    uint8_t controlLogic = !busPacket[2];
    
    if( controlLogic & B00000010 ) // BUSACK ?
    {
      // could add a check if we started the BUSRQ
      Serial.println ("BUSACK");
      
    }
    if ( controlLogic & B00000100 ) // WAIT ?
    {
      // Check if we started a WAIT operation
      Serial.println ("WAIT");
    }
    if (!getZ80HardlockState()) // Z80 Hardlock cleared ?
    {
      // Might have been Z80 releasing a Hardlock, check CACHE for changes
      Serial.println ("Z80 Hardlock Cleared");
    }

  }
  return intCode;
}

boolean establishESPHardlock (uint8_t retries) 
{
  do
  {
    if(getZ80HardlockState() == true) 
    { 
      delay(1); 
    }
    else
    { 
      Serial.println("ESP has Lock");
      setESPHardlock();
      return true;    // SUCCESSFUL LOCK
    }

  } while (--retries > 0);
  return false;       // FAILED TO LOCK
}

void releaseESPHardlock(void)
{
  Serial.println("ESP released Lock");
  clearESPHardlock();
}

bool sendBUSRQ(uint8_t retries)
{
  WriteDataBUSOperation(0xFF, CONTROLBYTE_BUSRQ_ENABLE, 0xFFFF);

  uint8_t *busPacket = NULL;
  uint8_t controlLogic = 0;
  do
  {
    busPacket = getBUSstate();
    controlLogic = ~busPacket[2];  // controlByte     
    if ((controlLogic & CONTROL_MASK_BUSACK) == CONTROL_MASK_BUSACK) // BUSACK ?
    {
      // could add a check if we started the BUSRQ
      if(DEBUG_BUSRQ_BUSACK) { Serial.println("BUSACK -- OK"); }
      return true;
    }
  } while (--retries > 0);

  if(DEBUG_BUSRQ_BUSACK) { Serial.println("BUSACK -- FAIL"); }
  return false;
}

bool clearBUSRQ(void)
{
  WriteDataBUSOperation(0xFF,CONTROLBYTE_BUSRQ_DISABLE,0xFFFF);
  return true;
}


//
// RIO ROM

void selectROMbank(uint8_t ROMbank) 
{
  setROMbank(ROMbank);
}

void enableHostROM(void)
{
  WriteDataBUSOperation(0xff,CONTROLBYTE_HOSTROM_ENABLE,0xFFFF);
}

void disableHostROM(void)
{
  WriteDataBUSOperation(0xff,CONTROLBYTE_HOSTROM_DISABLE,0xFFFF);
}

// Public
void enableRIO_ROMRW(uint8_t ROMbank)
{
    disableRIOProtection();
    selectROMbank(ROMbank);
    disableHostROM();
}

void enableRIO_ROMRD(uint8_t ROMbank)
{
    selectROMbank(ROMbank);
    disableHostROM();
}

void disableRIO_ROM(void)
{
    enableRIOProtection();
    enableHostROM();
}



//
// RIO IOdevice

void enableRIO_IOdConfigAccess(void)
{
    establishESPHardlock(3);
    disableRIOProtection();
}

void disableRIO_IOdConfigAccess(void)
{
    releaseESPHardlock();
    enableRIOProtection();
}


//
// SETUP THE HAL

void ESPHardware_setup(void) 
{
  Setup_ESPHALDriver();
  // PUSH IODevice config to RIO

  setBUSidle();
  releaseESPHardlock();
}