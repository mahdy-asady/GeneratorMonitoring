#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f1xx.h"

void timerInit(TIM_HandleTypeDef *tHandler, TIM_TypeDef *Timer);

void timerInputCaptureInit(TIM_HandleTypeDef *tHandler, uint32_t timerChannel, void (*callBack)(uint16_t));

void timerInputCaptureStart(TIM_HandleTypeDef *tHandler, uint32_t timerChannel);

void timerInputCaptureStop(TIM_HandleTypeDef *tHandler, uint32_t timerChannel);

#endif
