#ifndef __BOARD_H__
#define __BOARD_H__

#include "usart.h"
#include "i2c.h"


/* Initalize Board */
void boardInit(void);

void boardInitUsartEsp(usartHandle *usartHandler);

void boardInitUsartDebug(usartHandle *usartHandler);

void boardInitI2C(I2C_HandleTypeDef *i2cHandler);

void boardInitEsp(usartHandle *usartHandler);

void boardInitTimer(TIM_HandleTypeDef *timerHandler);

void boardInitADC(ADC_HandleTypeDef *adcHandler);

void boardToggleHealthLED(void);

uint8_t boardReadDigit1(void);

uint8_t boardReadDigit2(void);


#endif