#include <Arduino.h>
#include <SPI.h>

#include <ESPTests.h>
#include <Z80Tests.h>

/*
  PORT A -- Z80 DATA

  PORT B -- 0123 SPI

  PORT F -- Z80 ADDRESS LOW
  PORT K -- Z80 ADDRESS HIGH
*/

uint8_t *localPacket;

void printLocalPacket()
{
  Serial.print("localPacket : 0[");
  Serial.print(localPacket[0], HEX);
  Serial.print("] 1[");
  Serial.print(localPacket[1], HEX);
  Serial.print("] 2[");
  Serial.print(localPacket[2], HEX);
  Serial.print("] 3[");
  Serial.print(localPacket[3], HEX);
  Serial.println("]");
}




void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115000);
  Serial.println("Running.");

  pinMode(LED_BUILTIN, OUTPUT);

  // Set pin start-up states

  // ESP SPI
  SPI.begin();
  ESP_setup();
  

  // Z80
  Z80_setup();


}

void loop()
{
  // put your main code here, to run repeatedly:

  
    // Blinky code for enable to test if running
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    //delay(1000);
  

  
  ESP_CacheStatusTest();
  ESP_CacheData_Test();
  ESP_ROM_Test();
  ESP_IOd_Test();
  

  
  enableROM(0);
  Z80_ROMMemory_Test();
  enableROM(1);
  Z80_ROMMemory_Test();
  disableROM();
  enableROM(2);
  Z80_ROMMemory_Test();
  enableROM(3);
  Z80_ROMMemory_Test();
  disableROM();
  

  
  // IOD TEST
  ESP_FillCacheStatus(0xf0);
  ESP_test_configIOd();
  Z80_IORQ_Test();
  

}
