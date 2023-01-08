#include "MotionTracking.h"
#include "debug.h"

#define MPU6050_ADDR    0x68

#define REG_PWR_MGMT_1  0x6B
#define REG_WHO_AM_I    0x75

#define REG_ACCEL_X     0x3B
#define REG_ACCEL_Y     0x3D
#define REG_ACCEL_Z     0x3F

#define REG_GYRO_X      0x43
#define REG_GYRO_Y      0x45
#define REG_GYRO_Z      0x47

void MotionTrackingInit(I2C_HandleTypeDef *i2cHandler) {
    uint8_t resetOp = 0x00;

    if(!i2cWriteMemory(i2cHandler, MPU6050_ADDR, REG_PWR_MGMT_1, &resetOp, 1)) {
        debugError("I2C Write Error on MotionTracking");
        return ;
    }
}

MotionTrackingInfo MotionTrackingReadData(I2C_HandleTypeDef *i2cHandler) {
    MotionTrackingInfo result = {0};

    uint8_t i2cResult[14];

    if(!i2cReadMemory(i2cHandler, MPU6050_ADDR, REG_ACCEL_X, i2cResult, 14)) {
        debugError("I2C Read Error on MotionTracking");
    }

    result.accelX = (i2cResult[0] << 8) | (i2cResult[1] & 0xFC);
    result.accelY = (i2cResult[2] << 8) | (i2cResult[3] & 0xFC);
    result.accelZ = (i2cResult[4] << 8) | (i2cResult[5] & 0xFC);

    result.gyroX = (i2cResult[8] << 8) | (i2cResult[9] & 0xFC);
    result.gyroY = (i2cResult[10] << 8) | (i2cResult[11] & 0xFC);
    result.gyroZ = (i2cResult[12] << 8) | (i2cResult[13] & 0xFC);

    return result;
}
