
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
    delay(50);
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
    delay(50);
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

  uint8_t y = 0x00;
  for (uint16_t i = 0; i < 0x10; i++)
  {
    Serial.print("doROMWrite : ");
    Serial.print(y,HEX);
    Serial.print(",");
    Serial.print(i,HEX);
    // void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1
    doCEROMWrite(y, i, 0);
    
    data = doCEROMRead(i, 0);
    Serial.print(" - doROMRead : ");
    Serial.println(data,HEX);
    
    y++;
    delay(50);
  }

  // CLEAR
  delay(50);
}

void ESP_IOd_Test(void)
{
  Serial.println("\nESP_IOd_Test");
  uint8_t data;

  //clearBUS();
  delay(50);

  uint8_t y = 0x00;
  for (uint16_t i = 0; i < 0x10; i++)
  {
    Serial.print("doIOdWrite : ");
    Serial.print(y,HEX);
    Serial.print(",");
    Serial.print(i,HEX);
    // void doCacheStatusWrite(uint8_t Data, uint16_t Address); //A16-1
    doIOdWrite(y, i);
    
    data = doIOdRead(i);
    Serial.print(" - doIOdRead : ");
    Serial.println(data,HEX);
    
    y++;
    delay(50);
  }

  // CLEAR
  delay(50);
}

void ESP_FillCacheStatus (uint8_t data)
{
  Serial.println("\nESP_FillCacheStatus");
  Serial.print(data,HEX);

  for (uint16_t i = 0x0000; i != 0x000F; i++)  // 0xFFFF takes ages!
  {
    doCacheStatusWrite(data, i);
  }
      
}

void ESP_test_configIOd(void)
{
  Serial.println("\nESP_test_configIOd");
  doIOdWrite(IODCONFIG_STATUS,1);
  doIOdWrite(IODCONFIG_CACHE0,3);
  doIOdWrite(IODCONFIG_IOEND,5);
}