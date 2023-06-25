#include <ESPHardwareInterface.h>

boolean RUNSLOW = false;
int SPEED = 250;

uint8_t *BUSbytesPTR;
//  BUSbyte [3] = Data;
//  BUSbyte [2] = Control;
//  BUSbyte [1] = loAddr;
//  BUSbyte [0] = hiAddr;

void displayBUSPTR(uint8_t *BUSptr)
{
    Serial.print("\nBUSptr : ");
    Serial.print(BUSptr[0],HEX);
    Serial.print(" ah, ");
    Serial.print(BUSptr[1],HEX);
    Serial.print(" al, ");
    Serial.print(BUSptr[2],BIN);
    Serial.print(" c, ");
    Serial.print(BUSptr[3],BIN); 
    Serial.print(" ");
    Serial.print(BUSptr[3],HEX);    
    Serial.println(" d");
    return;
}


void ESP_setup(void)
{
  ESPHardware_setup();
  // ESP Warm up -- FUJI setup will be needed here at somepoint
  clearBUS();
  resetESPHardlock();
  // Program the IOd & ROMs

  return;
}



//
//  Z80 BUS Interactions
//

// Everything in here should be done under a BUSRQ which disables the Z80

// ESP accessing the ROM/IOd chip VIA Z80 Bus
// Uses these to read/write ROMs into the IOd chip
uint8_t doCEROMRead(uint16_t Address, uint8_t ROMbank) // A16-0
{
    doBUSRQ();
    selectROMbank(ROMbank);
    // Z80 CE
    //connectZ80Bus();

    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CEROMRQ_RD);
    displayBUSPTR(BUSbytesPTR);
    clearBUS();

    return BUSbytesPTR[3];
}

void doCEROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank) // A16-0
{
    doBUSRQ();
    selectROMbank(ROMbank);
    //connectZ80Bus();

    doBUSWrite(Data, Address, CONTROLBYTE_CEROMRQ_WR);

    if(RUNSLOW) delay(SPEED);
    clearBUS();
}

void enableROM(uint8_t ROMbank)
{
    selectROMbank(ROMbank);
    setROMCS();
}

void disableROM(void)
{
    resetROMCS();
}

// These methods are for programming the RIO configuration bytes
uint8_t doRIOconfigRead(uint16_t Address) // A16-1
{
    doBUSRQ();
    // connect Z80Data and ROM/IOdbus
    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_RIOCONFIG_RD);
    clearBUS();
    return BUSbytesPTR[3];
}

void doRIOconfigWrite(uint8_t Data, uint16_t Address) // A16-1
{
    doBUSRQ();
    // connect Z80Data and ROM/IOdbus
    doBUSWrite(Data, Address, CONTROLBYTE_RIOCONFIG_WR);
    clearBUS();
}

// ESP controlling the system in place of the Z80
// Direct access to the host system memory
uint8_t doZ80MEMRead(uint16_t Address)
{
    doBUSRQ();

    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_MEMRQ_RD);

    clearBUS();
    return BUSbytesPTR[3];
}

void doZ80MEMWrite(uint8_t Data, uint16_t Address)
{
    doBUSRQ();

    doBUSWrite(Data, Address, CONTROLBYTE_MEMRQ_WR);

    clearBUS();
}

// IO calls may be needed to access system memory bank registers etc
// Probably will not be much use for interacting with actual hardware (does not create true bus timings)
uint8_t doZ80IORead(uint16_t Address)
{
    doBUSRQ();

    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_IORQ_RD);

    clearBUS();
    return BUSbytesPTR[3];
}

void doZ80IOWrite(uint8_t Data, uint16_t Address)
{
    doBUSRQ();

    doBUSWrite(Data, Address, CONTROLBYTE_IORQ_WR);

    clearBUS();
}




//
//  LOCAL BUS Interactions
//

// All actions here should be done under ESP-HARDLOCK conditions
// The Z80 (Host) will not be able to lock or use the system while ESP is accessing it

// ESP accessing Cache chip
uint8_t doCacheDataRead(uint16_t Address) // A16-0
{
    setESPHardlock();

    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CACHEDATA_RD);

    clearBUS();
    resetESPHardlock();
    return BUSbytesPTR[3];
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) // A16-0
{
    setESPHardlock();

    doBUSWrite(Data, Address, CONTROLBYTE_CACHEDATA_WR);

    clearBUS();
    resetESPHardlock();
}

uint8_t doCacheStatusRead(uint16_t Address) // A16-1
{
    setESPHardlock();
    //displayBUSPTR(BUSbytesPTR);
    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CACHESTATUS_RD);
    clearBUS();
    resetESPHardlock();

//    displayBUSPTR(BUSbytesPTR);
    return BUSbytesPTR[3];
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) // A16-1
{
    setESPHardlock();

    doBUSWrite(Data, Address, CONTROLBYTE_CACHESTATUS_WR);
    
    clearBUS();
    resetESPHardlock();
}
