#include <Arduino.h>
#include <SPI.h>


#define PORT_DDR_OUTPUT 255
#define PORT_DDR_INPUT 0

#define SBUS_STC_out 49
#define SBUS_OE_out 48
#define ESP_HARDLOCK 5

#define Z80DATA PORTA
#define Z80DATA_DDR DDRA

#define Z80ADDR_L PORTF
#define Z80ADDR_L_DDR DDRF

#define Z80ADDR_H PORTK
#define Z80ADDR_H_DDR DDRK


/*
  PORT A -- Z80 DATA

  PORT B -- 0123 SPI
  
  PORT F -- Z80 ADDRESS LOW
  PORT K -- Z80 ADDRESS HIGH




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


  //Z80 Ports
  Z80DATA_DDR = PORT_DDR_OUTPUT;
  Z80ADDR_L_DDR = PORT_DDR_OUTPUT;
  Z80ADDR_H_DDR = PORT_DDR_OUTPUT;


}

void loop()
{
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  Z80DATA = 255;
  Z80ADDR_L = 255;
  Z80ADDR_H = 255;

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