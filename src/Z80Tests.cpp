#include <Z80Tests.h>

void Z80_ROMMemory_Test(void)
{
Serial.println("Z80_ROMMemory_Test : START ");  
    uint8_t data = 0;
//Serial.println("Z80 Memory WRITE ROM");
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
Serial.println("Z80_ROMMemory_Test : END ");  
  return;
}

void Z80_IORQ_Test(void)
{
  // do_Z80_IOWR(1,99);
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