#include "stm32f1xx_hal.h"
#include "adc.h"
#include "timer.h"

ADC_HandleTypeDef voltageAdcHandler;

TIM_HandleTypeDef *gTimerHandler;
uint32_t voltageChannel;


#include <stdio.h>

void generatorReadVoltage(uint16_t timerPulse) {
    timerOutputCompareStop(gTimerHandler, voltageChannel);
    printf("%d\n", adcRead(&voltageAdcHandler));
}

uint16_t generatorPreviousTick;
void generatorZeroCrossInterrupt(uint16_t PulseTick) {
    timerOutputCompareStart(gTimerHandler, voltageChannel);

    printf("Current: %d\tPrev: %d", PulseTick, generatorPreviousTick);
    uint16_t duration = PulseTick - generatorPreviousTick;
    generatorPreviousTick = PulseTick;
    printf("\tFreq: %d\n", 10000 / (duration * 2));
}

void generatorInit(ADC_TypeDef *ADC, uint32_t ADC_Channel, TIM_HandleTypeDef *tHandler, uint32_t zeroCrossTimerChannel, uint32_t voltageTimerChannel) {
    gTimerHandler = tHandler;
    voltageChannel = voltageTimerChannel;

    adcInit(&voltageAdcHandler, ADC, ADC_Channel);

    timerInputCaptureInit(tHandler, zeroCrossTimerChannel, &generatorZeroCrossInterrupt);
    timerOutputCompareInit(tHandler, voltageTimerChannel, &generatorReadVoltage);

    timerInputCaptureStart(tHandler, zeroCrossTimerChannel);
}
