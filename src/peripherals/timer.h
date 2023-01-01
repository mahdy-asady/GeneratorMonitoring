#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f1xx.h"

void timerInit(TIM_HandleTypeDef *tHandler, TIM_TypeDef *Timer);

void timer_IC_Init(TIM_HandleTypeDef *tHandler, uint32_t timerChannel);

#endif
