#include <Arduino.h>
#include <SPI.h>


#define SBUS_STC_out 49
#define SBUS_OE_out 48
#define ESP_HARDLOCK 5

/*
  PORT B -- 0123 SPI
  
  PORT F (low) & K (high) -- Z80 ADDRESS




*/



SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE3);

void setup()
{
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  SPI.begin();
  pinMode(SBUS_STC_out, OUTPUT);
  pinMode(SBUS_OE_out,OUTPUT);
  pinMode(ESP_HARDLOCK,OUTPUT);

  // Set pin start-up states
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_OE_out,HIGH);
  digitalWrite(ESP_HARDLOCK,HIGH);

}

void loop()
{
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);


  // Load the Shift Regs
  SPI.beginTransaction(mySpiSettings);
  SPI.transfer(255); // Addr HI
  SPI.transfer(255); // Addr LO
  SPI.transfer(255); // Z80 Control
  SPI.transfer(255); // Local Control
  SPI.transfer(255); // DATA
  SPI.endTransaction();

  // LATCH the Shift Regs to Storage
  digitalWrite(SBUS_STC_out, LOW);
  delay(1);
  digitalWrite(SBUS_STC_out, HIGH);
  

  // Enable the Output from the 595s
  digitalWrite(SBUS_OE_out,LOW);
  delay(1000);
  digitalWrite(SBUS_OE_out,HIGH);

}