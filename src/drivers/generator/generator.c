#include "stm32f1xx_hal.h"
#include "adc.h"

ADC_HandleTypeDef voltageAdcHandler;


void generatorInit(ADC_TypeDef *ADC, uint32_t channel) {
    adcInit(&voltageAdcHandler, ADC, channel);
}

#include <stdio.h>
uint16_t generatorPreviousTick;
void generatorZeroCrossInterrupt(uint16_t PulseTick) {
    printf("Current: %d\tPrev: %d", PulseTick, generatorPreviousTick);
    uint16_t duration = PulseTick - generatorPreviousTick;
    generatorPreviousTick = PulseTick;
    printf("\tFreq: %d\n", 10000 / (duration * 2));
}
