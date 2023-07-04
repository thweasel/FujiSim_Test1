#include <Z80Hardware_emulator.h>

# define SLOW true
# define SLOWRATE 100

#define PORT_DDR_OUTPUT 255
#define PORT_DDR_INPUT 0


// Z80 BUS Assignments
#define Z80_DATA_out PORTA
#define Z80_DATA_in  PINA
#define Z80_DATA_DDR DDRA

#define Z80_ADDR_L      PORTF
#define Z80_ADDR_L_DDR  DDRF

#define Z80_ADDR_H      PORTK
#define Z80_ADDR_H_DDR  DDRK

// Z80 Control Assignments
#define Z80_BUSACK 30   // OUTPUT -- Response to external devices BUSRQ
#define Z80_RD     21   // OUTPUT -- Read operation
#define Z80_WR     20   // OUTPUT -- Write operation (pulse)
#define Z80_IORQ   19   // OUTPUT -- IO Access operation
#define Z80_MEMRQ  18   // OUTPUT -- Memory Access operation
#define Z80_BUSRQ  17    // INPUT -- External device requesting control of the BUS
#define Z80_WAIT   16   // INPUT  -- Z80 enter WAIT mode
#define Z80_ROMCS  15   // INPUT  -- ZX spectrum ROM access (ROM access decoded Via ULA MEMRQ+A14+A15)
#define Z80_NMI    14   // INPUT  -- Z80 NMI trigger


#define Z80_SIM_ISR 2   // [INTERRUPT] 
// OTHER Z80 CPU PINS
//  M1      // OUTPUT -- Extend T-State
//  RFSH    // OUTPUT -- DRAM refresh?
//  HALT    // OUTPUT -- LOW when Z80 executes HALT instructions, Z80 waiting (in a NOP loop) for NMI or INT to occur
//  INT     // INPUT  -- Interupt request
//  RESET   // INPUT  -- Reset the Z80
//  CLK     // INPUT  -- Clock signal driving Z80


//
// Z80 OUTPUT Control line operations 
//

// SEND conditions should set pull-ups?
void sendZ80_RD(void)               {   pinMode(Z80_RD, OUTPUT);        digitalWrite(Z80_RD, LOW);      }
void clearZ80_RDactive(void)        {   pinMode(Z80_RD, OUTPUT);        digitalWrite(Z80_RD, HIGH);     }
void clearZ80_RDpassive(void)       {   pinMode(Z80_RD, INPUT);         digitalWrite(Z80_RD, LOW);      }

void sendZ80_WR(void)               {   pinMode(Z80_WR, OUTPUT);        digitalWrite(Z80_WR, LOW); digitalWrite(Z80_WR, HIGH);      }
void clearZ80_WRactive(void)        {   pinMode(Z80_WR, OUTPUT);        digitalWrite(Z80_WR, HIGH);     }
void clearZ80_WRpassive(void)       {   pinMode(Z80_WR, INPUT);         digitalWrite(Z80_WR, LOW);      }

void sendZ80_IORQ(void)             {   pinMode(Z80_IORQ, OUTPUT);      digitalWrite(Z80_IORQ, LOW);    }
void clearZ80_IORQactive(void)      {   pinMode(Z80_IORQ, OUTPUT);      digitalWrite(Z80_IORQ, HIGH);   }
void clearZ80_IORQpassive(void)     {   pinMode(Z80_IORQ, INPUT);       digitalWrite(Z80_IORQ, LOW);    }

void sendZ80_MEMRQ(void)            {   pinMode(Z80_MEMRQ, OUTPUT);     digitalWrite(Z80_MEMRQ, LOW);   }
void clearZ80_MEMRQactive(void)     {   pinMode(Z80_MEMRQ, OUTPUT);     digitalWrite(Z80_MEMRQ, HIGH);  }
void clearZ80_MEMRQpassive(void)    {   pinMode(Z80_MEMRQ, INPUT);      digitalWrite(Z80_MEMRQ, LOW);   }

void sendZ80_BUSACK(void)           {   pinMode(Z80_BUSACK, OUTPUT);    digitalWrite(Z80_BUSACK, LOW);  }
void clearZ80_BUSACK(void)          {   pinMode(Z80_BUSACK, OUTPUT);    digitalWrite(Z80_BUSACK, HIGH); }


//
// Z80 INPUT Control line operations (Return TRUE if LOW)
boolean isWAIT(void)     
{
    if(digitalRead(Z80_WAIT) == 0) { return true; }
    else { return false; }
}
//boolean checkINT(void)      {   return !digitalRead(Z80_INT)    ;}
boolean isNMI(void)      
{   
    if(digitalRead(Z80_NMI) == 0) { return true; }
    else { return false; }
}
//boolean checkRESET(void)    {   return !digitalRead(Z80_RESET); }
boolean isBUSRQ(void)    
{   
     if(digitalRead(Z80_BUSRQ) == 0) 
     { Serial.println("Z80_BUSRQ = 0");
        return true; }
     else { 
        Serial.println("Z80_BUSRQ = 1");
        return false; }
}
  

void enableZ80_ADDRpullups (void) 
{   
    // Pins pulled high with internal 10K (bus pulled up for input)
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;
    Z80_ADDR_L = 255;
    Z80_ADDR_H = 255;
    return;
}

void disableZ80_ADDRpullups (void) 
{
    // Tri-state the pins (disconnect from bus)
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;
    Z80_ADDR_L = 0;
    Z80_ADDR_H = 0;
    return;
}

void enableZ80_DATApullups (void)
{
    // Pins pulled high with internal 10K (bus pulled up for input)
    Z80_DATA_DDR = PORT_DDR_INPUT;
    Z80_DATA_out = 255;
}

void disableZ80_DATApullups (void)
{
    // Tri-state the pins (disconnect from bus)
    Z80_DATA_DDR = PORT_DDR_INPUT;
    Z80_DATA_out = 0;
}

void clearZ80_ADDRpassive(void)
{
    // Function leaves the ports in INPUT mode (Tri-state)
    disableZ80_ADDRpullups();    
    return;
}

void clearZ80_DATApassive(void)
{ 
    // Function leaves the ports in INPUT mode (Tri-state)
    disableZ80_DATApullups();
    return;
}

/*
void clearZ80_ADDRactive(void)
{
    // THIS FUNCTON ISN'T REPRESENTATIVE OF A Z80
    //  Address Bus (output, active High, tristate).

    // Function leaves the ports in OUTPUT mode ALL PINS HIGH
    Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_L = 255;
    Z80_ADDR_H = 255;
}

void clearZ80_DATAactive(void)
{
    // THIS FUNCTON ISN'T REPRESENTATIVE OF A Z80
    //  Address Bus (output, active High, tristate).
    // Function leaves the ports in OUTPUT mode ALL PINS HIGH
    Z80_DATA_DDR = PORT_DDR_OUTPUT;    
    Z80_DATA_out = 255;
}
*/


void setZ80_IDLEpassive(void)
{
    // When under BUSRQ the CPU TRI-STATES the controls and buses to enable another device to control the system

    // INPUT PINS UNCHANGED
    // Z80 OUTPUT pins set to Tri-state INPUTS the pins with Pull-up OFF (passive the outputs)

    // Z80 Control lines
    clearZ80_RDpassive();
    clearZ80_WRpassive();
    clearZ80_IORQpassive();
    clearZ80_MEMRQpassive();

    // Z80 BUS
    clearZ80_ADDRpassive();
    clearZ80_DATApassive();

    return;
}

void setZ80_IDLEactive(void)
{

    setZ80_IDLEpassive();
    // INPUT PINS UNCHANGED
    // Z80 OUTPUT pins HIGH

    // Z80 Control lines
    //clearZ80_RDactive();
    //clearZ80_WRactive();
    //clearZ80_IORQactive();
    //clearZ80_MEMRQactive();

    // Z80 BUS only Tri-States
    //clearZ80_ADDRpassive();
    //clearZ80_DATApassive();
    
    return;
}

void sendZ80_ADDR(uint16_t Address)
{
    Z80_ADDR_L_DDR = PORT_DDR_OUTPUT;
    Z80_ADDR_H_DDR = PORT_DDR_OUTPUT;

    Z80_ADDR_L = (uint8_t)Address;
    Z80_ADDR_H = (uint8_t)(Address >> 8);

    return;
}

uint16_t fetchZ80_ADDR(void)
{
    // This is not a native Z80 operation, but may be helpful to debug the system
    // SHOULD BE IN INPUT MODE ALREADY?!
    Z80_ADDR_L_DDR = PORT_DDR_INPUT;
    Z80_ADDR_H_DDR = PORT_DDR_INPUT;

    uint16_t address = 0;
    address = Z80_ADDR_H;
    address = address << 8;
    address = address | Z80_ADDR_L;

    clearZ80_ADDRpassive();
    return address;
}

void sendZ80_DATA(uint8_t Data)
{
    Z80_DATA_DDR = PORT_DDR_OUTPUT;    
    Z80_DATA_out = Data;  
    sendZ80_WR();
    return;

}

uint8_t fetchZ80_DATA(void)
{
    sendZ80_RD();
    Z80_DATA_DDR = PORT_DDR_INPUT;                  
    return Z80_DATA_in;
}

void Z80_IDLE() 
{
    // DEFAULT state between actions    
    setZ80_IDLEactive(); 
    if (SLOW) {delay(SLOWRATE);}   
}

void serviceBUSRQ (void) 
{   // Broken, bounces on CHANGE detection
    if(isBUSRQ())
    {
        setZ80_IDLEpassive();
        sendZ80_BUSACK();
    } 
    else 
    {
        clearZ80_BUSACK();
        setZ80_IDLEactive();
    }
    return;
}

void Z80Hardware_setup(void)
{
    clearZ80_RDactive();
    clearZ80_WRactive();
    clearZ80_IORQactive();
    clearZ80_MEMRQactive();
    clearZ80_BUSACK();

    // Z80 INPUT pin Config
    pinMode(Z80_WAIT, INPUT_PULLUP);
    pinMode(Z80_ROMCS, INPUT_PULLUP);
    pinMode(Z80_NMI, INPUT_PULLUP);
    pinMode(Z80_BUSRQ, INPUT_PULLUP);
  
    // TURN OFF PULL-UP RESISTORS FOR BUS (Tri-state no Pull-up)
    disableZ80_ADDRpullups();
    disableZ80_DATApullups();

    Z80_IDLE();

    // Interrupt pins -- Broken, bounces on CHANGE detection
    // attachInterrupt(digitalPinToInterrupt(Z80_SIM_ISR),serviceBUSRQ, CHANGE);

    return;
}
