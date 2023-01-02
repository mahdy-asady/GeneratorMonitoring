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

int16_t mpu6050ReadRegister(I2C_HandleTypeDef *i2cHandler, uint8_t registerAddr);

void MotionTrackingInit(I2C_HandleTypeDef *i2cHandler) {
    uint8_t resetOp = 0x00;

    if(!i2cWriteMemory(i2cHandler, MPU6050_ADDR, REG_PWR_MGMT_1, &resetOp, 1)) {
        debugError("I2C Write Error on MotionTracking");
        return ;
    }
}

int16_t MotionTrackingReadGyroX(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_GYRO_X);
}

int16_t MotionTrackingReadGyroY(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_GYRO_Y);
}

int16_t MotionTrackingReadGyroZ(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_GYRO_Z);
}


int16_t MotionTrackingReadAccX(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_ACCEL_X);
}

int16_t MotionTrackingReadAccY(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_ACCEL_Y);
}

int16_t MotionTrackingReadAccZ(I2C_HandleTypeDef *i2cHandler) {
    return mpu6050ReadRegister(i2cHandler, REG_ACCEL_Z);
}






int16_t mpu6050ReadRegister(I2C_HandleTypeDef *i2cHandler, uint8_t registerAddr) {
    uint8_t i2cResult[2];
    
    /*if(!i2cWrite(i2cHandler, MPU6050_ADDR, &registerAddr, 1)) {
        debugError("I2C Write Error on RTH");
        return 0;
    }*/

    if(!i2cReadMemory(i2cHandler, MPU6050_ADDR, registerAddr, i2cResult, 2)) {
        debugError("I2C Read Error on MotionTracking");
        return 0;
    }

    return (i2cResult[0] << 8) | (i2cResult[1] & 0xFC);
}
