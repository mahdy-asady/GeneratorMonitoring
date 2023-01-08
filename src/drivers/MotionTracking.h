#ifndef __MOTIONTRACKING_H__
#define __MOTIONTRACKING_H__

#include <stdint.h>
#include "i2c.h"

typedef struct  {
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;

    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;

} MotionTrackingInfo;


void MotionTrackingInit(I2C_HandleTypeDef *i2cHandler);

MotionTrackingInfo MotionTrackingReadData(I2C_HandleTypeDef *i2cHandler);

#endif