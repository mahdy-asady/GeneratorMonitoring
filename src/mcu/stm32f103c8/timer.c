#include "timer.h"
#include "debug.h"
#include "board.h"

TIM_HandleTypeDef *timerHandler;

void timerInit(TIM_HandleTypeDef *tHandler, TIM_TypeDef *Timer) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    tHandler->Instance = Timer;
    tHandler->Init.Prescaler = 36000-1;
    tHandler->Init.CounterMode = TIM_COUNTERMODE_UP;
    tHandler->Init.Period = 65535;
    tHandler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    tHandler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(tHandler) != HAL_OK) {
        debugFatal("Error Config TIMER!");
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(tHandler, &sClockSourceConfig) != HAL_OK) {
        debugFatal("Error Config TIMER!");
    }
    
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(tHandler, &sMasterConfig) != HAL_OK) {
        debugFatal("Error Config TIMER!");
    }

    timerHandler = tHandler;
}


void timer_IC_Init(TIM_HandleTypeDef *tHandler, uint32_t timerChannel) {
    if (HAL_TIM_IC_Init(tHandler) != HAL_OK) {
        debugFatal("Error Config TIMER!");
    }

    TIM_IC_InitTypeDef sConfigIC = {0};
    
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(tHandler, &sConfigIC, timerChannel) != HAL_OK) {
        debugFatal("Error Config TIMER!");
    }

    HAL_TIM_IC_Start_IT(timerHandler, timerChannel);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *tHandler) {
    if(tHandler->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        printf("Got pulse at: %ld\n", HAL_TIM_ReadCapturedValue(tHandler, TIM_CHANNEL_1));
}
