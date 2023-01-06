#ifndef __SENSETIMER_H__
#define __SENSETIMER_H__

void senseTimerInit(TIM_HandleTypeDef *timerHandler, uint32_t timerChannel, I2C_HandleTypeDef *i2cHandler);

#endif
