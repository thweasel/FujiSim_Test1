#include <ESPBUSInterface.h>


void ESP_CACHE_Status_Access_Test(void);
void ESP_CACHE_Data_Access_Test(void);
void ESP_RIO_ROM_Access_Test(void);
void ESP_RIO_IODconfig_Access_Test(void);

void ESP_FillCacheStatus (uint8_t data);
void ESP_RIOconfig_Cache0to0x0001(void);