#include <Arduino.h>
#include <SPI.h>

#define PORT_DDR_OUTPUT 255
#define PORT_DDR_INPUT 0

#define SBUS_STC_out  49
#define SBUS_OE_out   48
#define ESP_HARDLOCK  10
#define Z80_HARDLOCK   9
#define CONNECT_Z80    8

void clear_ESP_SBUS_out();