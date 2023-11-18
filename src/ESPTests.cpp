
#include "ESPTests.h"
#include "consoleDebug.h"



void ESP_CacheStatusTest(void)
{
  Serial.println("\nESP_CacheStatusTest");

  uint8_t data = 0x00;
  for (uint16_t addr = 0x0000; addr < 0x0001; addr++)
  {
    
    consoleShowAddrData("doCacheStatusWrite",addr, data,HEX);
    // void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1
    doCacheStatusWrite(data, addr);
    
    data = doCacheStatusRead(addr);
    consoleShowAddrData("doCacheStatusRead",addr, data,HEX);
    
    data++;
    //delay(50);
  }

}

void ESP_CacheData_Test(void)
{
  Serial.println("\nESP_CacheData_Test");
  
  uint8_t data = 0x00;
  for (uint16_t addr = 0; addr < 0x10; addr++)
  {

    consoleShowAddrData("doCacheDataWrite",addr, data,HEX);
    doCacheDataWrite(data, addr);
    data = doCacheDataRead(addr);
    consoleShowAddrData("doCacheDataRead",addr, data,HEX);
    
    data++;
  }

}

void ESP_ROM_Test(void)
{
  Serial.println("\nESP_ROM_Test");
  uint16_t errors =0;

  uint8_t writeData = 0x00;
  uint8_t readData = 0x00;
  for (uint16_t addr = 0x0000; addr < 0xFFFF; addr++)
  {
    //data = addr;
    writeData = writeData+1;

    
    doRIOROMWrite(writeData, addr, 0);

    readData = doRIOROMRead(addr, 0);
    
    if(writeData != readData)
    {
      consoleShowAddrData("doRIOROMWrite", addr, writeData, HEX);
      consoleShowAddrData("doRIOROMRead", addr, readData, HEX);
      errors++;
    }
  }
  if(errors !=0)
  {
    Serial.print("errors");
    Serial.println(errors);
  }
  else
  {
    Serial.println(" [ PASS ]  ESP_ROM_Test \n");
  }

}

void ESP_RIOconfig_Access_Test(void)
{
  Serial.println("\nESP_RIOconfig_Test");

  uint8_t data = 0x00;
  for (uint16_t addr = 0; addr < 0x10; addr++)
  {
    consoleShowAddrData("doRIOconfigWrite", addr, data, HEX);
    doRIOconfigWrite(data, addr);

    data = doRIOconfigRead(addr);
    consoleShowAddrData("doRIOconfigRead", addr, data, HEX);
    data++;
  }
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
  consoleShowAddrData("READ IODCONFIG_STATUS", 1, data, HEX);
  data = doRIOconfigRead(3); // 0x80
  consoleShowAddrData("READ IODCONFIG_CACHE0", 3, data, HEX);
  data = doRIOconfigRead(5); // 0x50
  consoleShowAddrData("READ IODCONFIG_IOEND", 5, data, HEX);
}