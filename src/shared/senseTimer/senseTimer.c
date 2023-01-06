#include "stm32f1xx_hal.h"

#include "timer.h"

TIM_HandleTypeDef *sTimerHandler;
uint32_t senseChannel;
uint16_t senseCounter;

void measureGyro(void) {

}

void measureTemperature(void) {

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


void senseTimerInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel) {
    senseCounter = 0;
    sTimerHandler = timerHandler;
    senseChannel = timerChannel;

    timerOutputCompareInit(timerHandler, timerChannel, &senseTimerInterrupt);
    timerOutputCompareStart(timerHandler, timerChannel, timerHandler->Instance->CNT + 1);
}
