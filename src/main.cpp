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

  //Serial.begin(115000);
  Serial.begin(9600);
  Serial.println("Running.");

  pinMode(LED_BUILTIN, OUTPUT);

  // Set pin start-up states

  // ESP SPI
  
  ESP_setup();
  

  // Z80
  Z80_setup();


}

void loop()
{
  Serial.println("\n\nTEST LOOP START");
  // put your main code here, to run repeatedly:

  
    // Blinky code for enable to test if running
    /*
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    */

  //
  // BASIC LOCAL ESP FUNCTION TESTING
  //

  //
  //  ESP Cache Tests
  //  
  ESP_CACHE_Status_Access_Test(); // WORKING (SIM)
  ESP_CACHE_Data_Access_Test();  // SIM FAILS on Addr 0?
  
  //
  //  ESP RIO function tests
  //
  ESP_RIO_ROM_Access_Test();
  ESP_RIO_IODconfig_Access_Test();  // SIM PASSES BUT RIO CONFIG EMPTY?  -- CANT READ ON VERSION 2 BOARD!
    


  // BASIC Z80 FUNCTION TESTING  

  //
  // RIO ROM Tests
  //
    
  enableRIO_ROMRW(0);
  Z80_ROMMemory_Test();
  disableRIO_ROM();
  /*
  enableRIO_ROMRW(1);
  Z80_ROMMemory_Test();
  enableRIO_ROMRW(2);
  Z80_ROMMemory_Test();
  enableRIO_ROMRW(3);
  Z80_ROMMemory_Test();
  disableRIO_ROM();
  */

    
//
// IODdevice TEST
//  


ESP_RIOconfig_Cache0to0x0001();
Z80_IORQ_Test();
  
Serial.println("\n\nTEST LOOP END");
}
