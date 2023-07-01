#include <Z80Hardware_emulation.h>

# define SLOW true
# define SLOWRATE 200


void Z80Hardware_setup(void)
{
  // Z80 OUTPUT pin Config
  pinMode(Z80_BUSACK, OUTPUT);
  pinMode(Z80_RD, OUTPUT);
  pinMode(Z80_WR, OUTPUT);
  pinMode(Z80_IORQ, OUTPUT);
  pinMode(Z80_MEMRQ, OUTPUT);

  // Z80 pin OUTPUT
  digitalWrite(Z80_BUSACK, HIGH);
  digitalWrite(Z80_RD, HIGH);
  digitalWrite(Z80_WR, HIGH);
  digitalWrite(Z80_IORQ, HIGH);
  digitalWrite(Z80_MEMRQ, HIGH);

  // Z80 INPUT pin Config
  pinMode(Z80_WAIT, INPUT);
  pinMode(Z80_ROMCS, INPUT);
  pinMode(Z80_NMI, INPUT);
  
  // Tri-state Pull-up OFF
  digitalWrite(Z80_WAIT, LOW);
  digitalWrite(Z80_ROMCS, LOW);
  digitalWrite(Z80_NMI, LOW);
  
  // TURN OFF PULL-UP RESISTORS FOR BUS (Tri-state no Pull-up)
  Z80_DATA_DDR = PORT_DDR_INPUT;
  Z80_ADDR_L_DDR = PORT_DDR_INPUT;
  Z80_ADDR_H_DDR = PORT_DDR_INPUT;
  Z80_DATA_out = 0;
  Z80_ADDR_L = 0;
  Z80_ADDR_H = 0;
  
  
  // Z80 Port Config (OUTPUT)
  Z80_DATA_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
  Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;
  
  // Z80 Port State (Default HIGH)
  Z80_DATA_out = 255;
  Z80_ADDR_L = 255;
  Z80_ADDR_H = 255;
  
  return;
}



void clear_Z80_ADDR()
{ // Default float address High?
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;
    return;
}

void set_Z80_ADDR(uint16_t Address)
{
    Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;

    Z80_ADDR_L = (uint8_t)Address;
    Z80_ADDR_H = (uint8_t)(Address >> 8);

    return;
}

uint16_t get_Z80_ADDR()
{
    // SHOULD BE IN INPUT MODE ALREADY?!
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;

    uint16_t address = 0;
    address = Z80_ADDR_H;
    address = address << 8;
    address = address | Z80_ADDR_L;

    clear_Z80_ADDR();
    return address;
}

void clear_Z80_DATA()
{ 
    Z80_DATA_DDR = PORT_DDR_INPUT;
    return;
}

void send_Z80_DATA(uint8_t Data)
{
    Z80_DATA_DDR = PORT_DDR_OUTPUT;
    
    Z80_DATA_out = Data;

    pinMode(Z80_WR,OUTPUT);
    digitalWrite(Z80_WR, LOW);
    
    //asm("nop; nop; nop; nop; nop; nop; nop; nop;");
    if (SLOW) {delay(SLOWRATE);}
    
    digitalWrite(Z80_WR, HIGH);

    return;
}

uint8_t read_Z80_DATA()
{
    Z80_DATA_DDR = PORT_DDR_INPUT;
    uint8_t data = 0;

    pinMode(Z80_RD,OUTPUT);
    digitalWrite(Z80_RD, LOW);
    
    //asm("nop; nop; nop; nop; nop; nop; nop; nop;");
    data = Z80_DATA_in;
    if (SLOW) {delay(SLOWRATE);}

    digitalWrite(Z80_RD, HIGH);    
    
    return data;
}

void Z80_IDLE()
{
    // Tri-state passive these outputs

    // Set pin to INPUT
    pinMode(Z80_RD, INPUT);
    pinMode(Z80_WR, INPUT);
    pinMode(Z80_IORQ, INPUT);
    pinMode(Z80_MEMRQ, INPUT);
    pinMode(Z80_BUSACK, INPUT);

    // Set pin Pull-up OFF
    digitalWrite(Z80_RD, LOW);
    digitalWrite(Z80_WR, LOW);
    digitalWrite(Z80_IORQ, LOW);
    digitalWrite(Z80_MEMRQ, LOW);
    digitalWrite(Z80_BUSACK, LOW);

    // INPUT PINS LEAVE ALONE
    //digitalWrite(Z80_WAIT, HIGH);
    //digitalWrite(Z80_ROMCS, HIGH);
    //digitalWrite(Z80_NMI, HIGH);

    clear_Z80_ADDR();
    clear_Z80_DATA();
    
    //asm("nop; nop; nop; nop; nop; nop; nop; nop;");
    if (SLOW) {delay(SLOWRATE);}
    return;
}

