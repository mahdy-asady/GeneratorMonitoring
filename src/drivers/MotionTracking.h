#ifndef __MOTIONTRACKING_H__
#define __MOTIONTRACKING_H__

#include <stdint.h>
#include "i2c.h"

void MotionTrackingInit(I2C_HandleTypeDef *i2cHandler);

int16_t MotionTrackingReadGyroX(I2C_HandleTypeDef *i2cHandler);
int16_t MotionTrackingReadGyroY(I2C_HandleTypeDef *i2cHandler);
int16_t MotionTrackingReadGyroZ(I2C_HandleTypeDef *i2cHandler);

int16_t MotionTrackingReadAccX(I2C_HandleTypeDef *i2cHandler);
int16_t MotionTrackingReadAccY(I2C_HandleTypeDef *i2cHandler);
int16_t MotionTrackingReadAccZ(I2C_HandleTypeDef *i2cHandler);

#endif