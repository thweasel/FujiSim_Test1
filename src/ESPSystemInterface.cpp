
#include "ESPemulation.h"

uint8_t *BUSbytesPTR;
//  BUSbyte [3] = Data;
//  BUSbyte [2] = Control;
//  BUSbyte [1] = loAddr;
//  BUSbyte [0] = hiAddr;

// ESP accessing the ROM/IOd chip VIA Z80 Bus
// Uses these to read/write ROMs into the IOd chip
uint8_t doCEROMRead(uint16_t Address, uint8_t ROMbank) // A16-0
{
    enableZ80ControlBus();
    selectROMbank(ROMbank);
    // Z80 CE
    //connectZ80Bus();

    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CEROMRQ_RD);

    clearBUS();

    return BUSbytesPTR[3];
}

void doCEROMWrite(uint8_t Data, uint16_t Address, uint8_t ROMbank) // A16-0
{
    enableZ80ControlBus();
    selectROMbank(ROMbank);
    //connectZ80Bus();

    doBUSWrite(Data, Address, CONTROLBYTE_CEROMRQ_WR);

    clearBUS();
}

// These methods are for programming the IOd configuration bytes
uint8_t doIOdRead(uint16_t Address) // A16-1
{
    enableZ80ControlBus();
    // connect Z80Data and ROM/IOdbus

    doBUSRead(Address, CONTROLBYTE_IORQ_RD);

    clearBUS();
    return 0;
}

void doIOdWrite(uint8_t Data, uint16_t Address) // A16-1
{
    enableZ80ControlBus();
    // connect Z80Data and ROM/IOdbus

    doBUSWrite(Data, Address, CONTROLBYTE_IORQ_WR);

    clearBUS();
}

// ESP controlling the system in place of the Z80
// Direct access to the host system memory
uint8_t doZ80MEMRead(uint16_t Address)
{
    enableZ80ControlBus();

    doBUSRead(Address, CONTROLBYTE_MEMRQ_RD);

    clearBUS();
    return 0;
}

void doZ80MEMWrite(uint8_t Data, uint16_t Address)
{
    enableZ80ControlBus();

    doBUSWrite(Data, Address, CONTROLBYTE_MEMRQ_WR);

    clearBUS();
}

// IO calls may be needed to access system memory bank registers etc
// Probably will not be much use for interacting with actual hardware (does not create true bus timings)
uint8_t doZ80IORead(uint16_t Address)
{
    enableZ80ControlBus();

    doBUSRead(Address, CONTROLBYTE_IORQ_RD);

    clearBUS();
    return 0;
}

void doZ80IOWrite(uint8_t Data, uint16_t Address)
{
    enableZ80ControlBus();

    doBUSWrite(Data, Address, CONTROLBYTE_IORQ_WR);

    clearBUS();
}


//
//  LOCAL BUS CONNECTED DEVICES
//

// ESP accessing Cache chip
uint8_t doCacheDataRead(uint16_t Address) // A16-0
{
    enableLocalControlBus();
    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CACHEDATA_RD);

    clearBUS();
    return BUSbytesPTR[3];
}

void doCacheDataWrite(uint8_t Data, uint16_t Address) // A16-0
{
    enableLocalControlBus();

    doBUSWrite(Data, Address, CONTROLBYTE_CACHEDATA_WR);

    clearBUS();
}

uint8_t doCacheStatusRead(uint16_t Address) // A16-1
{
    enableLocalControlBus();
    BUSbytesPTR = doBUSRead(Address, CONTROLBYTE_CACHESTATUS_RD);

    digitalWrite(CONTROL_Local, HIGH);

    clearBUS();
    return BUSbytesPTR[3];
}

void doCacheStatusWrite(uint8_t Data, uint16_t Address) // A16-1
{
    enableLocalControlBus();

    doBUSWrite(Data, Address, CONTROLBYTE_CACHESTATUS_WR);
    
    clearBUS();

}
