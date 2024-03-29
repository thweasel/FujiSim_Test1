#include <Z80Tests.h>

void Z80_ROMMemory_Test(void)
{
Serial.println("\nZ80_ROMMemory_Test");  
    uint8_t data = 0;
Serial.println("Z80 Memory WRITE ROM (0xF1,0xE2,0xD3)");
  do_Z80_MEMWR(1, 0xF1);
  do_Z80_MEMWR(2, 0xE2);
  do_Z80_MEMWR(3, 0xD3);

Serial.println("Z80 Memory READ ROM");  
  data = do_Z80_MEMRD(1);
  Serial.println(data,HEX);
  data = do_Z80_MEMRD(2);
  Serial.println(data,HEX);
  data = do_Z80_MEMRD(3);
  Serial.println(data,HEX);
  return;
}

void Z80_IO_Test(void) 
{
  // Basic IO space WR/RD test
  Serial.println("\nZ80_IO_Test");  
  uint8_t data = 0;
  Serial.println("Z80 IO space WRITE (0xF1,0xE2,0xD3)");
  do_Z80_IOWR(1, 0xF1);
  do_Z80_IOWR(2, 0xE2);
  do_Z80_IOWR(3, 0xD3);

  Serial.println("Z80 IO space READ");  
  data = do_Z80_IORD(1);
  Serial.println(data,HEX);
  data = do_Z80_IORD(2);
  Serial.println(data,HEX);
  data = do_Z80_IORD(3);
  Serial.println(data,HEX);
  return;

}

void Z80_IORQ_Test(void)
{
  uint8_t data = 0;
  Serial.println("\nZ80_IORQ_Test");  
  // do_Z80_IOWR(1,99);

  Serial.println("WRITE to Device 1-3-5");  
  //data = do_Z80_IORD(1);  // Lock on READ
  //Serial.print("Status:  ");
  //Serial.println(data,HEX);

  do_Z80_IOWR(1,0x44);   // lock on WRITE
  Serial.print("Write Start:  ");
  Serial.println(0x44,HEX);

  do_Z80_IOWR(3,0x01);
  Serial.print("Write 0:  ");
  Serial.println(0x01,HEX);
  
  do_Z80_IOWR(3,0x02);
  Serial.print("Write 1:  ");
  Serial.println(0x02,HEX);

  do_Z80_IOWR(3,0x03);
  Serial.print("Write 2:  ");
  Serial.println(0x03,HEX);

  do_Z80_IOWR(5,0x88);   // unlock on WRITE
  Serial.print("Write End:  ");
  Serial.println(0x88,HEX);


  Serial.println("READ from Device 1-3-5");  
  data = do_Z80_IORD(1);
  Serial.print("Read Start:  ");
  Serial.println(data,HEX);

  data = do_Z80_IORD(3);
  Serial.print("Read 0:  ");
  Serial.println(data,HEX);
  
  data = do_Z80_IORD(3);
  Serial.print("Read 1:  ");
  Serial.println(data,HEX);
  
  data = do_Z80_IORD(3);
  Serial.print("Read 2:  ");
  Serial.println(data,HEX);
  
  data = do_Z80_IORD(5);
  Serial.print("Read End:  ");
  Serial.println(data,HEX);

  return;
}