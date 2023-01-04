#ifndef __GENERATOR_H__
#define __GENERATOR_H__


void generatorInit(ADC_TypeDef *ADC, uint32_t channel);

void generatorZeroCrossInterrupt(uint16_t PulseTick);

#endif