#include <Z80Tests.h>

void Z80_ROMMemory_Test(void)
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