#include <Arduino.h>
#include <SPI.h>

#include <ESPemulation.h>
#include <Z80emulation.h>

/*
  PORT A -- Z80 DATA

  PORT B -- 0123 SPI

  PORT F -- Z80 ADDRESS LOW
  PORT K -- Z80 ADDRESS HIGH
*/

void setup()
{
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  // Set pin start-up states

  // ESP
  // ESP pin Config
  SPI.begin();
  pinMode(SBUS_STC_out, OUTPUT);
  pinMode(SBUS_OE_out, OUTPUT);
  pinMode(ESP_HARDLOCK, OUTPUT);
  pinMode(Z80_HARDLOCK, INPUT);
  pinMode(CONNECT_Z80, OUTPUT);
  // ESP pin State
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_OE_out, HIGH);
  digitalWrite(ESP_HARDLOCK, LOW); // Lock on HIGH
  digitalWrite(Z80_HARDLOCK, LOW); // Lock on HIGH
  digitalWrite(CONNECT_Z80, HIGH); // Connect on LOW (may be buffer with NOT?)

  clear_ESP_SBUS_out();

  // Z80

  // Z80 pin Config
  pinMode(Z80_RD, OUTPUT);
  pinMode(Z80_WR, OUTPUT);
  pinMode(Z80_IORQ, OUTPUT);
  pinMode(Z80_MEMRQ, OUTPUT);
  pinMode(Z80_WAIT, INPUT);
  pinMode(Z80_ROMCS, INPUT);
  pinMode(Z80_NMI, INPUT);
  // Z80 pin State
  digitalWrite(Z80_RD, HIGH);
  digitalWrite(Z80_WR, HIGH);
  digitalWrite(Z80_IORQ, HIGH);
  digitalWrite(Z80_MEMRQ, HIGH);
  digitalWrite(Z80_WAIT, HIGH);
  digitalWrite(Z80_ROMCS, HIGH);
  digitalWrite(Z80_NMI, HIGH);
  // Z80 Port Config
  Z80_DATA_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;
  // Z80 Port State
  Z80_DATA = 255;
  Z80_ADDR_L = 255;
  Z80_ADDR_H = 255;
}

void loop()
{
  // put your main code here, to run repeatedly:

  /*
    // Blinky code for enable to test if running
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  */

  //do_Z80_IOWR(1, 1);
  //do_Z80_IORD(1);

  //do_Z80_IOWR(2, 2);
  //do_Z80_IORD(2);

  do_Z80_MEMWR(3, 32);
  do_Z80_MEMRD(3);

}