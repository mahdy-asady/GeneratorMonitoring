#ifndef __GENERATOR_H__
#define __GENERATOR_H__


void generatorInit(ADC_TypeDef *ADC, uint32_t ADC_Channel, TIM_HandleTypeDef *timerHandler, uint32_t zeroCrossTimerChannel, uint32_t voltageTimerChannel);

void generatorReadVoltage(uint16_t timerPulse);

void generatorZeroCrossInterrupt(uint16_t PulseTick);

#endif