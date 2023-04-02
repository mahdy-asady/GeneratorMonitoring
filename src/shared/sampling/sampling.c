#include <stdio.h>
#include "sampling.h"
#include "fifo/fifo.h"

extern fifo32Data  frequencyFifo, dutyFifo, voltageFifo,

            gyroXFifo,  gyroYFifo,  gyroZFifo,
            accelXFifo, accelYFifo, accelZFifo,

            tempFifo, humidFifo,

            rpmFifo;

uint32_t getNormalizedValue(fifo32Data *Data) {
    uint64_t sum = 0;
    uint16_t count = 0;
    while(!fifo32IsEmpty(Data)) {
        count++;
        sum += fifo32Pop(Data);
    }
    return sum / count;
}

report samplingGenerateReport(void) {
    report reportData = {0};
    reportData.reportVersion = 1;


    reportData.mcuID = 0;
    reportData.powerVoltage = getNormalizedValue(&voltageFifo);
    reportData.powerDuty = getNormalizedValue(&dutyFifo);
    reportData.powerFrequency = getNormalizedValue(&frequencyFifo);
    reportData.areaTemperature = getNormalizedValue(&tempFifo);
    reportData.areaHumidity = getNormalizedValue(&humidFifo);
    reportData.motorRPM = getNormalizedValue(&rpmFifo);
    reportData.digital0 = 0;
    reportData.digital1 = 0;
    reportData.gyroX = getNormalizedValue(&gyroXFifo);
    reportData.gyroY = getNormalizedValue(&gyroYFifo);
    reportData.gyroZ = getNormalizedValue(&gyroZFifo);
    reportData.accX = getNormalizedValue(&accelXFifo);
    reportData.accY = getNormalizedValue(&accelYFifo);
    reportData.accZ = getNormalizedValue(&accelZFifo);
    reportData.checksum = 0;

    printf("Report:\n");
    printf("Voltage:%d\tDuty: %d\tFrequency:%d\n", reportData.powerVoltage, reportData.powerDuty, reportData.powerFrequency);
    printf("Temp:%d\tHumid: %d\t\n", reportData.areaTemperature, reportData.areaHumidity);
    printf("RPM:%d\tD0: %d\tD1:%d\n", reportData.motorRPM, reportData.digital0, reportData.digital1);
    return reportData;
}
