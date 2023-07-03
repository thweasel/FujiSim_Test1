
#include "ESPTests.h"

void ESP_CacheStatusTest(void)
{
  Serial.println("\nESP_CacheStatusTest");
  uint8_t data;

  //clearBUS();
  delay(50);

  uint8_t y = 0x00;
  for (uint16_t i = 0; i < 0x10; i++)
  {
    Serial.print("doCacheStatusWrite : ");
    Serial.print(y,HEX);
    Serial.print(",");
    Serial.print(i,HEX);
    // void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1
    doCacheStatusWrite(y, i);
    
    data = doCacheStatusRead(i);
    Serial.print(" - doCacheStatusRead : ");
    Serial.println(data,HEX);
    
    y++;
    //delay(50);
  }

  // CLEAR
  delay(50);
}

void ESP_CacheData_Test(void)
{
  Serial.println("\nESP_CacheData_Test");
  uint8_t data;

  //clearBUS();
  delay(50);

  uint8_t y = 0x00;
  for (uint16_t i = 0; i < 0x10; i++)
  {
    Serial.print("doCacheDataWrite : ");
    Serial.print(y,HEX);
    Serial.print(",");
    Serial.print(i,HEX);
    doCacheDataWrite(y, i);
    data = doCacheDataRead(i);
    Serial.print(" - doCacheDataRead : ");
    Serial.println(data,HEX);
    
    y++;
    //delay(50);
  }

  // CLEAR
  delay(50);
}

void ESP_ROM_Test(void)
{
  Serial.println("\nESP_ROM_Test");
  uint8_t data;

  //clearBUS();
  delay(50);

  data = 0x00;
  for (uint16_t addr = 0; addr < 0x10; addr++)
  {
    data=addr;
    //data = 0x0F;
    
    Serial.print("doROMWrite : data ");
    Serial.print(data,HEX);
    Serial.print(", addr ");
    Serial.println(addr,HEX);
    doRIOROMWrite(data, addr, 0);
    

    data = doRIOROMRead(addr, 0);
    Serial.print(" - doROMRead : ");
    Serial.println(data,HEX);
    
    
    //delay(50);
    //delay(1000);
  }

  // CLEAR
  delay(50);
}

void ESP_RIOconfig_Access_Test(void)
{
  Serial.println("\nESP_RIOconfig_Test");
  uint8_t data;

  //clearBUS();
  delay(50);

  uint8_t y = 0x00;
  for (uint16_t i = 0; i < 0x10; i++)
  {
    Serial.print("doRIOconfigWrite : ");
    Serial.print(y,HEX);
    Serial.print(",");
    Serial.print(i,HEX);
    // void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1
    doRIOconfigWrite(y, i);
    
    data = doRIOconfigRead(i);
    Serial.print(" - doRIOconfigRead : ");
    Serial.println(data,HEX);
    
    y++;
    //delay(50);
  }

  // CLEAR
  delay(50);
}

void ESP_FillCacheStatus (uint8_t data)
{
  Serial.println("\nESP_FillCacheStatus");
  Serial.print(data,HEX);

  uint8_t check = 0;

  for (uint16_t i = 0x0000; i != 0x000F; i++)  // 0xFFFF takes ages!
  {
    doCacheStatusWrite(data, i);
    check = doCacheStatusRead(i);
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