#ifndef __RPM_H_
#define __RPM_H_

#include <stdint.h>

#include "timer.h"

void rpmInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel);

void rpmPulseInterrupt(uint16_t PulseTick);

#endif