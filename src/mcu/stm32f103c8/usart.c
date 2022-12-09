#include "usart.h"
#include "debug.h"
#include "retarget/retarget.h"

#include "stm32f1xx_hal_uart.h"

#define USART_COUNT     3
usartHandle *usartHandlers[USART_COUNT];

bool findHandler(UART_HandleTypeDef *HAL_Handler, usartHandle *handler) {
    switch ((uint32_t)HAL_Handler->Instance) {
        case (uint32_t)USART1:
            handler = usartHandlers[0];
            break;
        case (uint32_t)USART2:
            handler = usartHandlers[1];
            break;
        
        default:
            return false;
            break;
    }
    return true;
}

bool usartInit(usartHandle *handle, USART_TypeDef *usartInstance, uint32_t baudRate) {
    fifoInit(&handle->buffer);

    handle->HAL_Handler.Instance = usartInstance;
    handle->HAL_Handler.Init.BaudRate = baudRate;
    handle->HAL_Handler.Init.WordLength = UART_WORDLENGTH_8B;
    handle->HAL_Handler.Init.StopBits = UART_STOPBITS_1;
    handle->HAL_Handler.Init.Parity = UART_PARITY_NONE;
    handle->HAL_Handler.Init.Mode = UART_MODE_TX_RX;
    handle->HAL_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handle->HAL_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&handle->HAL_Handler) != HAL_OK) {
        debugFatal("Error Config USART!");
    }

    switch ((uint32_t)usartInstance) {
        case (uint32_t)USART1:
            usartHandlers[0] = handle;
            break;
        case (uint32_t)USART2:
            usartHandlers[1] = handle;
            break;
        
        default:
            debugFatal("Wrong USART selected!");
            break;
    }
    HAL_UART_Receive_IT (&handle->HAL_Handler, &handle->txBuffer, 1);
    return true;
}


void usartEnableDebug(usartHandle *handle) {
    RetargetInit(&handle->HAL_Handler);
}


/* USART data reception intrrupt handler */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
    usartHandle *handle = 0;
    if(findHandler(huart, handle)) {
        HAL_UART_Receive_IT(&handle->HAL_Handler, &handle->txBuffer, 1);
        //call buffer function
        if(!fifoIsFull(&handle->buffer))
            fifoPush(&handle->buffer, handle->txBuffer);
    }
}

void USART1_IRQHandler(void)
{
    if(usartHandlers[0])
        HAL_UART_IRQHandler(&usartHandlers[0]->HAL_Handler);
}