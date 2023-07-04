
#include "ESPTests.h"
#include "consoleDebug.h"

void ESP_CacheStatusTest(void)
{
  Serial.println("\nESP_CacheStatusTest");

  uint8_t data = 0x00;
  for (uint16_t addr = 0; addr < 0x10; addr++)
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
    Serial.print("doCacheDataWrite : ");
    Serial.print(data,HEX);
    Serial.print(",");
    Serial.print(addr,HEX);
    doCacheDataWrite(data, addr);
    data = doCacheDataRead(addr);
    Serial.print(" - doCacheDataRead : ");
    Serial.println(data,HEX);
    
    data++;
  }

}

void ESP_ROM_Test(void)
{
  Serial.println("\nESP_ROM_Test");

  uint8_t data = 0x00;
  for (uint16_t addr = 0; addr < 0x10; addr++)
  {
    data = addr;

    consoleShowAddrData("doRIOROMWrite", addr, data, HEX);
    doRIOROMWrite(data, addr, 0);

    data = doRIOROMRead(addr, 0);
    consoleShowAddrData("doRIOROMRead", addr, data, HEX);
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
  Serial.println("\nESP_FillCacheStatus");
  Serial.print(data,HEX);

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
  Serial.println("\nESP_test_configIOd");
  doRIOconfigWrite(IODCONFIG_STATUS,1); //0x90
  doRIOconfigWrite(IODCONFIG_CACHE0,3); //0x80
  doRIOconfigWrite(IODCONFIG_IOEND,5);  //0x50
}