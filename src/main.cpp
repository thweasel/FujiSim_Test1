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

void Z80_ROMMemory_Test (void)
{
  do_Z80_MEMWR(1, 16);
  do_Z80_MEMWR(2, 32);
  do_Z80_MEMWR(3, 64);

  do_Z80_MEMRD(1);
  do_Z80_MEMRD(2);
  do_Z80_MEMRD(3);
  
  return;
}


void Z80_IORQ_Test(void)
{
  //do_Z80_IOWR(1,99);
  do_Z80_IORD(1);
  do_Z80_IOWR(3, 1);
  do_Z80_IOWR(3, 2);
  do_Z80_IOWR(3, 4);
  do_Z80_IORD(5);

  do_Z80_IORD(1);
  do_Z80_IORD(3);
  do_Z80_IORD(3);
  do_Z80_IORD(3);
  do_Z80_IORD(5);

  return;
}

void ESP_Bus_test(void)
{
  digitalWrite(CONTROL_Local,LOW);

  clearBUS();
  delay(500);

  doIOWrite(1,0);
  delay(500);

  doIORead(1);
  delay(500);

  doMEMWrite(128,0);
  delay(500);
  
  doMEMRead(2);
  delay(500);
  digitalWrite(CONTROL_Local,HIGH);
}

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
  pinMode(ESP_PULSE, OUTPUT);
  pinMode(CONTROL_Local,OUTPUT);
  pinMode(CONTROL_Z80,OUTPUT);
  pinMode(ESP_HARDLOCK, OUTPUT);
  pinMode(Z80_HARDLOCK, INPUT);
  pinMode(CONNECT_Z80, OUTPUT);

  // ESP pin State
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_OE_out, HIGH);
  digitalWrite(ESP_PULSE,HIGH);
  digitalWrite(CONTROL_Local,HIGH);
  digitalWrite(CONTROL_Z80,HIGH);
  digitalWrite(ESP_HARDLOCK, HIGH); // Lock set LOW
  digitalWrite(Z80_HARDLOCK, HIGH); // Lock set LOW
  digitalWrite(CONNECT_Z80, HIGH); // Connect on LOW (may be buffer with NOT?)

  clearBUS();

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

  ESP_Bus_test();

// Z80_ROMMemory_Test();
// Z80_IORQ_Test();

}

