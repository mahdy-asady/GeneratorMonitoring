#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "adc.h"
#include "timer.h"
#include "fifo/fifo.h"

ADC_HandleTypeDef voltageAdcHandler;

TIM_HandleTypeDef *gTimerHandler;
uint32_t voltageChannel;
uint16_t generatorPreviousTick;

fifo32Data  frequencyFifo,
            dutyFifo,
            voltageFifo;

void generatorReadVoltage(uint16_t timerPulse) {
    timerOutputCompareStop(gTimerHandler, voltageChannel);
    if(!fifo32IsFull(&voltageFifo))
        fifo32Push(&voltageFifo, adcRead(&voltageAdcHandler));
}

void generatorZeroCrossInterrupt(uint16_t PulseTick) {
    timerOutputCompareStart(gTimerHandler, voltageChannel, PulseTick + 50);

    uint16_t duration = PulseTick - generatorPreviousTick;
    generatorPreviousTick = PulseTick;
    if(!fifo32IsFull(&frequencyFifo))
        fifo32Push(&frequencyFifo, 10000 / (duration * 2));
}

void generatorInit(ADC_TypeDef *ADC, uint32_t ADC_Channel, TIM_HandleTypeDef *tHandler, uint32_t zeroCrossTimerChannel, uint32_t voltageTimerChannel) {
    gTimerHandler = tHandler;
    voltageChannel = voltageTimerChannel;

    adcInit(&voltageAdcHandler, ADC, ADC_Channel);

    timerInputCaptureInit(tHandler, zeroCrossTimerChannel, &generatorZeroCrossInterrupt);
    timerOutputCompareInit(tHandler, voltageTimerChannel, &generatorReadVoltage);

    timerInputCaptureStart(tHandler, zeroCrossTimerChannel);
}
