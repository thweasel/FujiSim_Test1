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
  //Serial.begin(9600);
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
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    //delay(1000);
  
  ESP_ROM_Test();
  //ESP_RIOconfig_Access_Test();
  
  //ESP_CacheStatusTest();
  //ESP_CacheData_Test();


  
  enableRIO_ROMRW(0);
  Z80_ROMMemory_Test();
  disableRIO_ROM();
  
  enableRIO_ROMRW(1);
  Z80_ROMMemory_Test();
  enableRIO_ROMRW(2);
  Z80_ROMMemory_Test();
  enableRIO_ROMRW(3);
  Z80_ROMMemory_Test();
  disableRIO_ROM();
  

    
  // IOD TEST#
  
  
//  ESP_FillCacheStatus(0xf0);
//  ESP_RIOconfig_Cache0to0x0001();
//  Z80_IORQ_Test();
  

}
