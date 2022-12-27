#ifndef __ESP_H__
#define __ESP_H__

#include <stdbool.h>
#include "usart.h"

void espInit(usartHandle *handle);

bool espWifiConnect(char *SSID, char *Password);


#endif