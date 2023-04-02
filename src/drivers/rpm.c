#include "rpm.h"
#include "fifo/fifo.h"

uint16_t prevTick;

extern fifo32Data rpmFifo;


void rpmPulseInterrupt(uint16_t PulseTick) {
    uint16_t duration = PulseTick - prevTick;
    prevTick = PulseTick;

    if(!fifo32IsFull(&rpmFifo))
            fifo32Push(&rpmFifo, 60 * 10000 / duration);
}

void rpmInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel) {
    fifo32Init(&rpmFifo);
    
    timerInputCaptureInit(timerHandler, timerChannel, &rpmPulseInterrupt);
    timerInputCaptureStart(timerHandler, timerChannel);
}
