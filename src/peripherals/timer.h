#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f1xx.h"

void timerInit(TIM_HandleTypeDef *tHandler, TIM_TypeDef *Timer);

#endif
