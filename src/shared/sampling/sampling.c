#include <stdio.h>

#include "fifo/fifo.h"

fifo32Data  frequencyFifo, dutyFifo, voltageFifo,

            gyroXFifo,  gyroYFifo,  gyroZFifo,
            accelXFifo, accelYFifo, accelZFifo,

            tempFifo, humidFifo,

            rpmFifo;

uint32_t samplingGetNormalizedValue(fifo32Data *Data) {
    return 0;
}

void samplingGenerateReport(void) {
    printf("Report\n");
}
