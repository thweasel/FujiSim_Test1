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
  // Default IDLE state for the system -- MASK control bits BUSRQ/WAIT/ROMCS/NMI?
  
  WriteDataBUSOperation(0xff,0xff,0xffff);  // set all shift register outputs high
  return;
}

uint8_t doReadBUSData(uint16_t Address, uint8_t Control)
{
  uint8_t result;
  result = ReadDataBUSOperation(Control, Address);
  setBUSidle(); // Implicit as SPI output is cut off?
  return result;
}

void doWriteBUSData(uint8_t Data, uint16_t Address, uint8_t Control)
{
  WriteDataBUSOperation(Data,Control,Address);
  setBUSidle(); // Implicit as SPI output is cut off?
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

boolean getLocalBus (uint8_t retries) 
{
  // Establish a ESP Hard Lock to block incoming Z80 IORQ
  // Do not let RIO Decode IO while the SPI is using the Local Address bus via SPI!
  if(retries > 0)
  {
    do
    {
      if(getZ80HardlockState() == true) 
      { 
        delay(1); 
      }
      else
      { 
        //Serial.println("ESP has Lock");
        setESPHardlock();
        //sendBusSignalsFromSPI();  // CONTROLLED FROM !RIO_IO_DECODE 
        return true;    // SUCCESSFUL LOCK
      }

    } while (--retries > 0);
  }
  Serial.println("ESP failed to HardLock -- can't get Local Bus");
  return false;       // FAILED TO LOCK
}

void releaseLocalBus(void)
{
  //Serial.println("ESP released Lock");
  //setBUSidle();
  //stopBusSignalsFromSPI(); // CONTROLLED FROM !RIO_IO_DECODE 
  clearESPHardlock();
}

bool getZ80BUS(uint8_t retries)
{
  // Generate a Z80BUSRQ and wait for the HOST to reply Z80BUSACK
  // Z80BUS will be release implicitly when LocalBus is Released
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

/*
bool releaseZ80BUS(void)  // IMPLICIT when bus IDLEs
{
  WriteDataBUSOperation(0xFF,CONTROLBYTE_BUSRQ_DISABLE,0xFFFF);
  return true;
}
*/


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
    //getLocalBus(3);
    disableRIOProtection();
}

void disableRIO_IOdConfigAccess(void)
{
    //releaseLocalBus();
    enableRIOProtection();
}


//
// SETUP THE HAL

void ESPHardware_setup(void) 
{
  Setup_ESPHALDriver();
  // PUSH IODevice config to RIO

  // Prepare the Local BUS and Release control



  setESPHardlock();

  // WAIT for the Z80 Hardlock to reset
  do
  {
      delay(10); 
  } while (getZ80HardlockState() == true) ;

  setBUSidle();  
  sendBusSignalsFromSPI();

  clearESPHardlock();

  sendRESET();
}


