#include <string.h>

#include "usart.h"
#include "debug.h"
#include "retarget/retarget.h"
#include "config.h"
#include "stm32f1xx_hal_uart.h"

usartHandle *usartHandlers[USART_COUNT];

bool findHandler(UART_HandleTypeDef *HAL_Handler, usartHandle **handler) {
    switch ((uint32_t)HAL_Handler->Instance) {
        case (uint32_t)USART1:
            *handler = usartHandlers[0];
            break;
        case (uint32_t)USART2:
            *handler = usartHandlers[1];
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
    HAL_UART_Receive_IT (&handle->HAL_Handler, &handle->rxCharBuffer, 1);
    return true;
}


void usartEnableDebug(usartHandle *handle) {
    RetargetInit(&handle->HAL_Handler);
}


/* USART data reception intrrupt handler */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
    usartHandle *handle = NULL;
    if(findHandler(huart, &handle)) {
        HAL_UART_Receive_IT(&handle->HAL_Handler, &handle->rxCharBuffer, 1);
        //call buffer function
        if(!fifoIsFull(&handle->buffer))
            fifoPush(&handle->buffer, handle->rxCharBuffer);
        
    }
}

void usartWrite(usartHandle *handle, uint8_t *Data, uint16_t Size) {
    HAL_UART_Transmit(&handle->HAL_Handler, Data, Size, 100);
}

void usartWriteLine(usartHandle *handle, char *Text) {
    usartWrite(handle, (uint8_t *)Text, strlen(Text));
    usartWrite(handle, (uint8_t *)"\r\n", 2);
}

uint16_t usartRead(usartHandle *handle, uint8_t *Buffer, uint16_t MaxSize, uint16_t Timeout) {
    uint32_t TickStart = HAL_GetTick();
    uint16_t Size = 0;
    uint8_t *BufferPointer = Buffer;
    
    while(((HAL_GetTick() - TickStart) <= Timeout) && Size < MaxSize) {
        if(!fifoIsEmpty(&handle->buffer)) {
            *(BufferPointer++) = fifoPop(&handle->buffer);
            Size++;
        }
    }
    return Size;
}


uint16_t usartReadLine(usartHandle *handle, char *Buffer, uint16_t MaxSize, uint16_t Timeout) {
    uint32_t TickStart = HAL_GetTick();
    uint16_t Size = 0;
    char *BufferPointer = Buffer;

    while(((HAL_GetTick() - TickStart) <= Timeout) && Size < MaxSize) {
        if(!fifoIsEmpty(&handle->buffer)) {
            *BufferPointer = (char)fifoPop(&handle->buffer);
            
            if(*BufferPointer == '\n') {
                //remove \r\n
                *BufferPointer = 0;
                if(*(BufferPointer - 1) == '\r') {
                    Size--;
                    *(BufferPointer - 1) = 0;
                }
                break;
            }

            Size++;
            BufferPointer++;
        }
    }
    return Size;
}
