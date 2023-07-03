#include "ESPHALDriverATmega.h"
#include "ESPBUSInterface.h"
#include "ESPHAL.h"

// Should the HAL track the last bus state written or read
//uint8_t dataRegister;
//uint8_t controlRegister;
//uint16_t addressRegister;

uint8_t controlByte;  // Control bus state

//
// BASIC bus operations exposed to BUS interface

void IDLE_BUS(void)
{
  // Default IDLE state for the system
  stopBusSignals();
  return;
}

uint8_t doReadBUSData(uint16_t Address, uint8_t Control)
{
  uint8_t result;
  result = ReadDataBUSOperation(Control, Address);
  IDLE_BUS();
  return result;
}

void doWriteBUSData(uint8_t Data, uint16_t Address, uint8_t Control)
{
  WriteDataBUSOperation(Data,Control,Address);
  IDLE_BUS();
  return;
}

void ESPHardware_setup(void) 
{
  ESPHAL_setup();
  IDLE_BUS();
}

uint8_t pollINT(void) {

  // Control INPUT  [ RD | WE | IORQ | MEMRQ | BUSRQ | WAIT | BUSACK | NMI ]
  uint8_t intCode = 0;
  if( getINT() )
  {
    uint8_t *busPacket = getBUSstate(); // BusPacket [2] Control byte
    uint8_t control = !busPacket[2];
    
    if( control & B00000010 ) // BUSACK ?
    {
      // could add a check if we started the BUSRQ
      Serial.println ("BUSACK");
      
    }
    if ( control & B00000100 ) // WAIT ?
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

boolean establishESPHardlock (uint8_t waitus) 
{
  do
  {
    if(getZ80HardlockState() == true) 
    { 
      delay(1); 
    }
    else
    { 
      setESPHardlock();
      return true;    // SUCCESSFUL LOCK
    }

  } while (--waitus > 0);
  return false;       // FAILED TO LOCK
}

bool sendBUSRQ(uint8_t waitus)
{
  WriteDataBUSOperation(0xFF, CONTROLBYTE_BUSRQ_ENABLE, 0xFFFF);
  // WAIT HERE FOR THE BUSACK signal

  uint8_t *busPacket = NULL;
  uint8_t control = 0;
  do
  {
    
    busPacket = getBUSstate();
    control = ~busPacket[2];  // controlByte     
    /*   
    Serial.print("Waiting for BUSACK : control ");
    Serial.print(CONTROL_MASK_BUSACK,BIN);
    Serial.print(" & ");
    Serial.print(control,BIN);
    Serial.print(" = ");
    Serial.println((control & CONTROL_MASK_BUSACK),BIN);
    */
    if ((control & CONTROL_MASK_BUSACK) == CONTROL_MASK_BUSACK) // BUSACK ?
    {
      // could add a check if we started the BUSRQ
      Serial.println("BUSACK -- OK");
      return true;
    }
    delay(1);
  } while (--waitus > 0);
  
  //return true; // test setup
  return false;
}

bool clearBUSRQ(void)
{
  WriteDataBUSOperation(0xFF,CONTROLBYTE_BUSRQ_DISABLE,0xFFFF);
  
  // WAIT HERE FOR THE BUSACK signal
  delay(10);
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

void enableRIO_IOdConfigWR(void)
{

}



