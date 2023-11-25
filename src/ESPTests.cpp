
#include "ESPTests.h"
#include "consoleDebug.h"

#define DEBUG_DETAILED false

uint16_t errors =0;
uint8_t writeData = 0;
uint8_t readData = 0;


void initWriteReadTest (const char * TestName)
{
  Serial.print("\nTESTING : ");
  Serial.print(TestName);
  Serial.print(" >>  ");
  errors = 0;
  writeData = 1;
  readData = 1;
}

void logWriteReadTest(uint16_t addr, uint8_t writeData, uint8_t readData)
{
    if(writeData != readData)
    {
      if(DEBUG_DETAILED)
      {
        Serial.println ("\n Error! ");
        consoleShowAddrData("  Write data ", addr, writeData, HEX);
        consoleShowAddrData("  Read data ", addr, readData, HEX);
      }
      errors++;
    }
}

void reportWriteReadTest(void)
{

  if(errors !=0)
  {
    Serial.print("[ FAILED ]\n   Errors counted: ");
    Serial.println(errors);
  }
  else
  {
    Serial.println("[ PASSED ] ");
  }
  
}



void ESP_CACHE_Status_Access_Test(void)
{
  
  initWriteReadTest("ESP_CACHE_Status_Access_Test");

  for (uint16_t addr = 0x0000; addr < 0x00FF; addr++)
  {
    
    doCacheStatusWrite(writeData, addr);
    readData = doCacheStatusRead(addr);

    logWriteReadTest(addr,writeData,readData);
        
    writeData++;
    
  }
  reportWriteReadTest();

}

void ESP_CACHE_Data_Access_Test(void)
{
  initWriteReadTest("ESP_CACHE_Data_Access_Test");
  
  for (uint16_t addr = 0x0000; addr < 0x00FF; addr++)
  {    
    doCacheDataWrite(writeData, addr);
    readData = doCacheDataRead(addr);

    logWriteReadTest(addr,writeData,readData);
       
    writeData++;
  }
  reportWriteReadTest();

}

void ESP_RIO_ROM_Access_Test(void)
{
  initWriteReadTest("ESP_RIO_ROM_Access_Test");

  for (uint16_t addr = 0x0000; addr < 0x00FF; addr++)
  {
    writeData = writeData+1;
    
    doRIOROMWrite(writeData, addr, 0);
    readData = doRIOROMRead(addr, 0);

    logWriteReadTest(addr,writeData,readData);

  }
  reportWriteReadTest();
}

void ESP_RIO_IODconfig_Access_Test(void)
{
  initWriteReadTest("ESP_RIO_IODconfig_Access_Test");

  for (uint16_t addr = 0; addr < 0x00FF; addr++)
  {
    
    doRIOconfigWrite(writeData, addr);
    readData = doRIOconfigRead(addr);
    //consoleShowAddrData("\nESP_RIO_IODconfig_Access_Test readData:", addr, readData, DEC);  // SIMULTATION DEBUGGING
    logWriteReadTest(addr,writeData,readData);
    writeData++;
  }
  reportWriteReadTest();
}

void ESP_FillCacheStatus (uint8_t data)
{
  Serial.print("\nESP_FillCacheStatus : ");
  Serial.println(data,HEX);

  uint8_t check = 0;

  for (uint16_t addr = 0x0000; addr != 0x000F; addr++)  // 0xFFFF takes ages!
  {
    doCacheStatusWrite(data, addr);
    check = doCacheStatusRead(addr);
    if(check != data) {
      Serial.print("\nESP_FillCacheStatus - Write error [");
      Serial.print(check);
      Serial.print('-');
      Serial.print(data);
      Serial.println(']');
      }
  }
      
}

void ESP_RIOconfig_Cache0to0x0001(void)
{
  uint8_t data;
  Serial.println("\nESP_test_configIOd");
  doRIOconfigWrite(IODCONFIG_STATUS, 1); // 0x90
  doRIOconfigWrite(IODCONFIG_CACHE0, 3); // 0x80
  doRIOconfigWrite(IODCONFIG_IOEND, 5);  // 0x50

  data = doRIOconfigRead(1); // 0x90
  consoleShowAddrData("READ IODCONFIG_STATUS ", 1, data, HEX);
  data = doRIOconfigRead(3); // 0x80
  consoleShowAddrData("READ IODCONFIG_CACHE  ", 3, data, HEX);
  data = doRIOconfigRead(5); // 0x50
  consoleShowAddrData("READ IODCONFIG_IOEND  ", 5, data, HEX);
}