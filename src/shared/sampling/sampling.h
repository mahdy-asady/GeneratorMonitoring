#ifndef __SAMPLING_H__
#define __SAMPLING_H__

#include <stdint.h>

#include "fifo/fifo.h"

typedef struct {
    uint8_t reportVersion;
    uint8_t packetID;
    uint32_t mcuID;
    uint16_t powerVoltage;
    uint8_t powerDuty;
    uint8_t powerFrequency;
    uint16_t areaTemperature;
    uint16_t areaHumidity;
    uint16_t motorRPM;
    bool digital0;
    bool digital1;
    uint16_t gyroX;
    uint16_t gyroY;
    uint16_t gyroZ;
    uint16_t accX;
    uint16_t accY;
    uint16_t accZ;
    uint32_t checksum;
}report;

report samplingGenerateReport(void);

#endif
