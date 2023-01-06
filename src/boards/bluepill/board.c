#include "stm32f1xx_hal.h"

#include "board.h"
#include "clock.h"
#include "timer.h"
#include "esp/esp.h"
#include "rpm.h"
#include "MotionTracking.h"
#include "generator.h"
#include "senseTimer/senseTimer.h"


void boardGpioInit(void) {
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pins : PB4 PB5 */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /*  Pin B9: Health LED
        Pin B8: ESP Reset
    */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void boardInit(usartHandle *usartDebugHandler, usartHandle *usartEspHandler, I2C_HandleTypeDef *i2cHandler) {
    // Init system...
    HAL_Init();
    clockConfig();
    boardGpioInit();

    //init usart for debug communication
    usartInit(usartDebugHandler, USART2, 115200);
    usartEnableDebug(usartDebugHandler);

    //init usart for ESP32 communication
    usartInit(usartEspHandler, USART1, 115200);

    //init I2C communication
    i2cInit(i2cHandler, I2C1);
    MotionTrackingInit(i2cHandler);

    espInit(usartEspHandler, GPIOB, GPIO_PIN_8);
}

void boardInitTimer(TIM_HandleTypeDef *timerHandler) {
    timerInit(timerHandler, TIM2);
    
    generatorInit(ADC1, ADC_CHANNEL_4, timerHandler, TIM_CHANNEL_2, TIM_CHANNEL_3);
    
    timerInputCaptureInit(timerHandler, TIM_CHANNEL_1, &rpmPulseInterrupt);
    timerInputCaptureStart(timerHandler, TIM_CHANNEL_1);

    senseTimerInit(timerHandler, TIM_CHANNEL_4);
}

void boardToggleHealthLED(void) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
}

uint8_t boardReadDigit1(void) {
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
}

uint8_t boardReadDigit2(void) {
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
}
