#ifndef __ESP_H__
#define __ESP_H__

#include <stdbool.h>
#include "usart.h"

void espInit(usartHandle *handle, GPIO_TypeDef *ResetGPIO, uint16_t ResetPin);

bool espWifiConnect(char *SSID, char *Password);


#endif