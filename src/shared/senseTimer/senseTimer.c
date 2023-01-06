#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "timer.h"
#include "MotionTracking.h"
#include "rht.h"

TIM_HandleTypeDef *sTimerHandler;
I2C_HandleTypeDef *sI2cHandler;
uint32_t senseChannel;
uint16_t senseCounter;

void measureGyro(void) {
    //printf("Gyro:\tX: %d\tY:%d\tZ:%d",  MotionTrackingReadGyroX(sI2cHandler), 
    //                                    MotionTrackingReadGyroY(sI2cHandler), 
    //                                    MotionTrackingReadGyroZ(sI2cHandler));
    //
    //printf("\t\tAccel:\tX: %d\tY:%d\tZ:%d\n",  MotionTrackingReadAccX(sI2cHandler), 
    //                                        MotionTrackingReadAccY(sI2cHandler), 
    //                                        MotionTrackingReadAccZ(sI2cHandler));
    printf("Gyro\n");
}

void measureTemperature(void) {
    printf("Temperature data: %d\n", rhtReadTemerature(sI2cHandler));
    printf("Humidity data: %d\n", rhtReadHumidity(sI2cHandler));
}

void measurePowerDetails(void) {

}

void senseTimerInterrupt(uint16_t counterPulse) {
    timerOutputCompareStart(sTimerHandler, senseChannel, counterPulse + 10);

    if(++senseCounter == 1000)
        senseCounter = 0;

    measureGyro();

    if((senseCounter % 100) == 0) {
        measureTemperature();
    }
    
    if((senseCounter % 10) == 0) {
        measurePowerDetails();
    }
}


void senseTimerInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel, I2C_HandleTypeDef *i2cHandler) {
    senseCounter = 0;
    sTimerHandler = timerHandler;
    senseChannel = timerChannel;
    sI2cHandler = i2cHandler;

    timerOutputCompareInit(timerHandler, timerChannel, &senseTimerInterrupt);
    timerOutputCompareStart(timerHandler, timerChannel, timerHandler->Instance->CNT + 10);
}
