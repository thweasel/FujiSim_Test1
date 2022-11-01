#include <ESPemulation.h>

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