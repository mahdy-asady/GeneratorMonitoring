#include "stm32f1xx_hal.h"

#include "board.h"
#include "clock.h"
#include "timer.h"
#include "esp/esp.h"


void boardGpioInit(void) {
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
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

void boardInit(void) {
    HAL_Init();
    clockConfig();
    boardGpioInit();
}

void boardInitUsartEsp(usartHandle *usartHandler) {
    usartInit(usartHandler, USART1, 115200);
}

void boardInitUsartDebug(usartHandle *usartHandler) {
    usartInit(usartHandler, USART2, 115200);
    usartEnableDebug(usartHandler);
}

void boardInitI2C(I2C_HandleTypeDef *i2cHandler) {
    i2cInit(i2cHandler, I2C1);
}

void boardInitEsp(usartHandle *usartHandler) {
    espInit(usartHandler, GPIOB, GPIO_PIN_8);
}

void boardInitTimer(TIM_HandleTypeDef *timerHandler) {
    timerInit(timerHandler, TIM2);
    timer_IC_Init(timerHandler, TIM_CHANNEL_1);
}

void boardToggleHealthLED(void) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
}
