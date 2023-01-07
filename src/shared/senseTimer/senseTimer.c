#include "stm32f1xx_hal.h"
#include "config.h"
#include "timer.h"
#include "MotionTracking.h"
#include "rht.h"
#include "fifo/fifo.h"

TIM_HandleTypeDef *sTimerHandler;
I2C_HandleTypeDef *sI2cHandler;
uint32_t senseChannel;
uint16_t senseCounter;

fifo32Data  gyroXFifo,  gyroYFifo,  gyroZFifo,
            accelXFifo, accelYFifo, accelZFifo,
            tempFifo, humidFifo;

void measureGyro(void) {
    if(!fifo32IsFull(&gyroXFifo))
        fifo32Push(&gyroXFifo, MotionTrackingReadGyroX(sI2cHandler));
    if(!fifo32IsFull(&gyroYFifo))
        fifo32Push(&gyroYFifo, MotionTrackingReadGyroY(sI2cHandler));
    if(!fifo32IsFull(&gyroZFifo))
        fifo32Push(&gyroZFifo, MotionTrackingReadGyroZ(sI2cHandler));

    if(!fifo32IsFull(&accelXFifo))
        fifo32Push(&accelXFifo, MotionTrackingReadAccX(sI2cHandler));
    if(!fifo32IsFull(&accelYFifo))
        fifo32Push(&accelYFifo, MotionTrackingReadAccY(sI2cHandler));
    if(!fifo32IsFull(&accelZFifo))
        fifo32Push(&accelZFifo, MotionTrackingReadAccZ(sI2cHandler));
}

void measureTemperature(void) {
    if(!fifo32IsFull(&tempFifo))
        fifo32Push(&tempFifo, rhtReadTemerature(sI2cHandler));

    if(!fifo32IsFull(&humidFifo))
        fifo32Push(&humidFifo, rhtReadHumidity(sI2cHandler));
}

void senseTimerInterrupt(uint16_t counterPulse) {
    timerOutputCompareStart(sTimerHandler, senseChannel, counterPulse + SENSING_GAP);

    measureGyro();

    if((senseCounter % SENSING_TEMP_PERIOD) == 0) {
        senseCounter = 0;
        measureTemperature();
    }
}


void senseTimerInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel, I2C_HandleTypeDef *i2cHandler) {
    senseCounter = 0;
    sTimerHandler = timerHandler;
    senseChannel = timerChannel;
    sI2cHandler = i2cHandler;

    fifo32Init(&gyroXFifo);
    fifo32Init(&gyroYFifo);
    fifo32Init(&gyroZFifo);

    fifo32Init(&accelXFifo);
    fifo32Init(&accelYFifo);
    fifo32Init(&accelZFifo);

    fifo32Init(&tempFifo);
    fifo32Init(&humidFifo);

    timerOutputCompareInit(timerHandler, timerChannel, &senseTimerInterrupt);
    timerOutputCompareStart(timerHandler, timerChannel, timerHandler->Instance->CNT + SENSING_GAP);
}
