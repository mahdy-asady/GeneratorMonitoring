#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "clock.h"
#include "usart.h"
#include "esp/esp.h"


static void GPIO_Init(void);

/* Main routine*/
int main(void) {
    HAL_Init();
    clockConfig();

    GPIO_Init();

    usartHandle usartESP = {0},
                usartDebug = {0};
    usartInit(&usartESP, USART1, 115200);
    usartInit(&usartDebug, USART2, 115200);
    usartEnableDebug(&usartDebug);
    printf("Hello!!!\n");

    espInit(&usartESP);

    while(1){
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    }
}


static void GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
