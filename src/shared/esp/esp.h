#ifndef __ESP_H__
#define __ESP_H__

#include <stdbool.h>
#include "usart.h"

void espInit(usartHandle *handle, GPIO_TypeDef *ResetGPIO, uint16_t ResetPin);

bool espWifiConnect(char *SSID, char *Password);

void espStartPassThroughUDP(char *ServerAddress, uint16_t ServerPort, uint16_t LocalPort);

void espStopPassThroughUDP(void);

#endif