#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f1xx.h"

void adcInit(ADC_HandleTypeDef *adcHandler, ADC_TypeDef *adcPort, uint32_t adcChannel);

uint16_t adcRead(ADC_HandleTypeDef *adcHandler);


#endif
