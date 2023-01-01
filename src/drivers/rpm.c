#include "rpm.h"

uint16_t prevTick;
#include <stdio.h>
void rpmPulseInterrupt(uint16_t PulseTick) {
    uint16_t duration = PulseTick - prevTick;
    prevTick = PulseTick;
    printf("Got pulse at: %d \tRPM: %d\n", duration,  60 * 10000 / duration);
}