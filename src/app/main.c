#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "clock.h"
#include "usart.h"
#include "esp/esp.h"


enum MessageTypes {
    MessageAck = 1,
    MessageData
};

typedef struct {
    enum MessageTypes MessageType;
    uint8_t Data;
} ServerMessage;


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

    espInit(&usartESP, GPIOB, GPIO_PIN_8);
    espWifiConnect("esp32", "123456789");
    espStartPassThroughUDP("192.168.11.157", 4000, 4000);

    //empty usart buffer
    usartFlushBuffer(&usartESP);
    
    //just a test
    usartWrite(&usartESP, (uint8_t *)"012345678901234567890123456789012345", 36);

    while(1){
        ServerMessage ReceptionBuffer;
        if(usartRead(&usartESP, (uint8_t *)&ReceptionBuffer, 2, 500)) {
            printf("Message received: %d, %d\n", ReceptionBuffer.MessageType, ReceptionBuffer.Data);
        }
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


    //Reset ESP Pin
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

    /*Configure GPIO pin : PB8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


}
