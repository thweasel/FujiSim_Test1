#include <Arduino.h>
#include <SPI.h>


/*
  PORT A -- Z80 DATA

  PORT B -- 0123 SPI
  
  PORT F -- Z80 ADDRESS LOW
  PORT K -- Z80 ADDRESS HIGH




*/

#define PORT_DDR_OUTPUT 255
#define PORT_DDR_INPUT 0

#define SBUS_STC_out  49
#define SBUS_OE_out   48
#define ESP_HARDLOCK  10
#define Z80_HARDLOCK   9

#define CONNECT_Z80    8


// Z80 Emulation
#define Z80_DATA PORTA
#define Z80_DATA_DDR DDRA

#define Z80_ADDR_L PORTF
#define Z80_ADDR_L_DDR DDRF

#define Z80_ADDR_H PORTK
#define Z80_ADDR_H_DDR DDRK

#define Z80_RD    21
#define Z80_WR    20
#define Z80_IORQ  19
#define Z80_MEMRQ 18
#define Z80_BUSRQ 17
#define Z80_WAIT  16
#define Z80_ROMCS 15
#define Z80_NMI   14

SPISettings mySpiSettings (4000000,MSBFIRST,SPI_MODE3);
void clear_ESP_SBUS_out()
{
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
}

void Z80_WR_Pulse()
{
  digitalWrite(Z80_WR,LOW);
  //_delay_us;
  delay(1000);
  digitalWrite(Z80_WR,HIGH);
}

void Z80_RD_Pulse()
{
  digitalWrite(Z80_RD,LOW);
  //_delay_us;
  delay(1000);
  digitalWrite(Z80_RD,HIGH);
}


void set_Z80_ADDR(uint16_t Address)
{
  Z80_ADDR_L = (uint8_t) Address;
  Z80_ADDR_H = (uint8_t) (Address >> 8);

}

void clear_Z80_ADDR()
{ // Default float address High?
  Z80_ADDR_L = 255;
  Z80_ADDR_H = 255;
}

void set_Z80_Data(uint8_t Data)
{
  Z80_DATA = Data;
}

void clear_Z80_DATA()
{ // Default float data High?
  Z80_DATA = 255;
}

void do_Z80_IOWR(uint16_t Address, uint8_t Data)
{
  digitalWrite(Z80_IORQ,LOW);
  set_Z80_ADDR(Address);
  set_Z80_Data(Data);
  Z80_WR_Pulse();
  digitalWrite(Z80_IORQ,HIGH);
  clear_Z80_ADDR();
  clear_Z80_DATA();
  digitalWrite(Z80_IORQ,HIGH);
  delay(1000);

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
  pinMode(SBUS_OE_out,OUTPUT);
  pinMode(ESP_HARDLOCK,OUTPUT);
  pinMode(Z80_HARDLOCK,INPUT);
  pinMode(CONNECT_Z80,OUTPUT);
  // ESP pin State
  digitalWrite(SBUS_STC_out, HIGH);
  digitalWrite(SBUS_OE_out,HIGH);
  digitalWrite(ESP_HARDLOCK,LOW); // Lock on HIGH
  digitalWrite(Z80_HARDLOCK,LOW); // Lock on HIGH
  digitalWrite(CONNECT_Z80,HIGH);  // Connect on LOW (may be buffer with NOT?)

  clear_ESP_SBUS_out();


  // Z80

  // Z80 pin Config
  pinMode(Z80_RD,OUTPUT);
  pinMode(Z80_WR,OUTPUT);
  pinMode(Z80_IORQ,OUTPUT);
  pinMode(Z80_MEMRQ,OUTPUT);
  pinMode(Z80_WAIT,INPUT);
  pinMode(Z80_ROMCS,INPUT);
  pinMode(Z80_NMI,INPUT);
  // Z80 pin State
  digitalWrite(Z80_RD,HIGH);
  digitalWrite(Z80_WR,HIGH);
  digitalWrite(Z80_IORQ,HIGH);
  digitalWrite(Z80_MEMRQ,HIGH);
  digitalWrite(Z80_WAIT,HIGH);
  digitalWrite(Z80_ROMCS,HIGH);
  digitalWrite(Z80_NMI,HIGH);
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


  do_Z80_IOWR(1,255);

  do_Z80_IOWR(2,255);

}