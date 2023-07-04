#include <ESPBUSInterface.h>

uint8_t data;

uint8_t *BUSpacket;
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

  // Program the IOd & ROMs

  return;
}



//
//  Z80 BUS Interactions
//

// Everything in here should be done under a BUSRQ which disables the Z80

// ESP accessing the ROM/IOd chip VIA Z80 Bus
// Uses these to read/write ROMs into the IOd chip
uint8_t doRIOROMRead(uint16_t Address, uint8_t ROMbank) // A16-0
{
    sendBUSRQ(3);
    enableRIO_ROMRD(ROMbank);
    
    data = doReadBUSData(Address, CONTROLBYTE_CEROMRQ_RD);
    setBUSidle();
    disableRIO_ROM();

    return data;
}

void doRIOROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank) // A16-0
{        
    if ( sendBUSRQ(3) ) 
    {        
        enableRIO_ROMRW(ROMbank);
    
        doWriteBUSData(Data, Address, CONTROLBYTE_CEROMRQ_WR);
        setBUSidle();
        disableRIO_ROM();
    }

}



// These methods are for programming the RIO configuration bytes
uint8_t doRIOconfigRead(uint16_t Address) // A16-1
{
    sendBUSRQ(); 

    // connect Z80Data and ROM/IOdbus
    data = doReadBUSData(Address, CONTROLBYTE_RIOCONFIG_RD);
    setBUSidle();
    return data;
}

void doRIOconfigWrite(uint8_t Data, uint16_t Address) // A16-1
{
    sendBUSRQ();
    enableRIO_IOdConfigWR();
    
    // connect Z80Data and ROM/IOdbus   
    doWriteBUSData(Data, Address, CONTROLBYTE_RIOCONFIG_WR);
    setBUSidle();
}

// ESP controlling the system in place of the Z80
// Direct access to the host system memory
uint8_t doZ80MEMRead(uint16_t Address)
{
    sendBUSRQ();

    data = doReadBUSData(Address, CONTROLBYTE_MEMRQ_RD);
    setBUSidle();
    
    return data;
}

void doZ80MEMWrite(uint8_t Data, uint16_t Address)
{
    sendBUSRQ();

    doWriteBUSData(Data, Address, CONTROLBYTE_MEMRQ_WR);
    setBUSidle();
    
}

// IO calls may be needed to access system memory bank registers etc
// Probably will not be much use for interacting with actual hardware (does not create true bus timings)
uint8_t doZ80IORead(uint16_t Address)
{
    sendBUSRQ();

    data = doReadBUSData(Address, CONTROLBYTE_IORQ_RD);
    setBUSidle();
    return data; // Data Byte
}

void doZ80IOWrite(uint8_t Data, uint16_t Address)
{
    sendBUSRQ();

    doWriteBUSData(Data, Address, CONTROLBYTE_IORQ_WR);
    setBUSidle();
}




//
//  LOCAL BUS Interactions
//
// All actions here should be done under ESP-HARDLOCK conditions
// The Z80 (Host) will not be able to lock or use the system while ESP is accessing it

//
// ESP accessing Cache chip
uint8_t doCacheDataRead(uint16_t Address) // A16-0
{
    if(establishESPHardlock(1))
    {
        data = doReadBUSData(Address, CONTROLBYTE_CACHEDATA_RD);
        return data;
    }
    return 0;
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) // A16-0
{
    establishESPHardlock(1);

    doWriteBUSData(Data, Address, CONTROLBYTE_CACHEDATA_WR);
    setBUSidle();
}

uint8_t doCacheStatusRead(uint16_t Address) // A16-1
{
    establishESPHardlock(1);
    //displayBUSPTR(BUSpacket);

    data = doReadBUSData(Address, CONTROLBYTE_CACHESTATUS_RD);
    setBUSidle();

//    displayBUSPTR(BUSpacket);
    return data;
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) // A16-1
{
    establishESPHardlock(1);

    doWriteBUSData(Data, Address, CONTROLBYTE_CACHESTATUS_WR);
    setBUSidle();

}
