#include "timer.h"
#include "debug.h"
#include "board.h"

TIM_HandleTypeDef *timerHandler[4];

void (*TimerInterruptCallbacks[16])(uint16_t);


uint8_t getTimerIndex(TIM_TypeDef *Timer) {
    uint8_t timerIndex;
    switch ((uint32_t)Timer)
    {
        case TIM1_BASE:
            timerIndex = 0; break;
        case TIM2_BASE:
            timerIndex = 1; break;
        case TIM3_BASE:
            timerIndex = 2; break;
        case TIM4_BASE:
            timerIndex = 3; break;
        default:
            debugFatal("Timer index out of range!");
    }
    return timerIndex;
}

void timerInit(TIM_HandleTypeDef *tHandler, TIM_TypeDef *Timer) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    tHandler->Instance = Timer;
    tHandler->Init.Prescaler = 7200-1;
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

    
    timerHandler[getTimerIndex(Timer)] = tHandler;
}


void timer_IC_Init(TIM_HandleTypeDef *tHandler, uint32_t timerChannel, void (*callBack)(uint16_t)) {
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

    HAL_TIM_IC_Start_IT(tHandler, timerChannel);

    uint8_t channelIndex;
    switch (timerChannel)
    {
        case TIM_CHANNEL_1:
            channelIndex = 0; break;
        case TIM_CHANNEL_2:
            channelIndex = 1; break;
        case TIM_CHANNEL_3:
            channelIndex = 2; break;
        case TIM_CHANNEL_4:
            channelIndex = 3; break;
        default:
            debugFatal("Timer Channel index out of range!");
    }
    uint8_t interruptIndex = getTimerIndex(tHandler->Instance) * 4 + channelIndex;

    TimerInterruptCallbacks[interruptIndex] = callBack;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *tHandler) {
    uint8_t Channel;
    switch (tHandler->Channel)
    {
        case HAL_TIM_ACTIVE_CHANNEL_1:
            Channel = TIM_CHANNEL_1; break;
        case HAL_TIM_ACTIVE_CHANNEL_2:
            Channel = TIM_CHANNEL_2; break;
        case HAL_TIM_ACTIVE_CHANNEL_3:
            Channel = TIM_CHANNEL_3; break;
        case HAL_TIM_ACTIVE_CHANNEL_4:
            Channel = TIM_CHANNEL_4; break;
        default:
            debugFatal("Timer Channel index out of range!");
    }

    uint8_t interruptIndex = getTimerIndex(tHandler->Instance) * 4 + tHandler->Channel - 1;
    uint16_t ChannelValue = HAL_TIM_ReadCapturedValue(tHandler, Channel);

    TimerInterruptCallbacks[interruptIndex](ChannelValue);
}
