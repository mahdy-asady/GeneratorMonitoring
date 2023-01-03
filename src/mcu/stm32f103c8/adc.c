#include "stm32f1xx_hal.h"
#include "debug.h"
#include "config.h"

void adcInit(ADC_HandleTypeDef *adcHandler, ADC_TypeDef *adcPort, uint32_t adcChannel) {
    ADC_ChannelConfTypeDef sConfig = {0};

    /** Common config
     */
    adcHandler->Instance = adcPort;
    adcHandler->Init.ScanConvMode = ADC_SCAN_DISABLE;
    adcHandler->Init.ContinuousConvMode = DISABLE;
    adcHandler->Init.DiscontinuousConvMode = DISABLE;
    adcHandler->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adcHandler->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adcHandler->Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(adcHandler) != HAL_OK)
    {
        debugFatal("Error Config ADC!");
    }

    /** Configure Regular Channel
     */
    sConfig.Channel = adcChannel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if (HAL_ADC_ConfigChannel(adcHandler, &sConfig) != HAL_OK)
    {
        debugFatal("Error Config ADC!");
    }
}

uint16_t adcRead(ADC_HandleTypeDef *adcHandler) {
    HAL_ADC_Start(adcHandler);
    HAL_ADC_PollForConversion(adcHandler, ADC_TIMEOUT);
    return HAL_ADC_GetValue(adcHandler);
}
