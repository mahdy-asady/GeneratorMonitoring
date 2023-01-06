#ifndef __BOARD_H__
#define __BOARD_H__

#include "usart.h"
#include "i2c.h"


/* Initalize Board */
void boardInit(usartHandle *usartDebugHandler, usartHandle *usartEspHandler, I2C_HandleTypeDef *i2cHandler, TIM_HandleTypeDef *timerHandler);

void boardToggleHealthLED(void);

uint8_t boardReadDigit1(void);

uint8_t boardReadDigit2(void);


#endif