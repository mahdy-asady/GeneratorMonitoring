#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>
#include <stdbool.h>

#include "stm32f1xx.h"
#include "fifo/fifo.h"

typedef struct {
    //USART_TypeDef  *instance;
    UART_HandleTypeDef  HAL_Handler;
    fifoData            buffer;
    uint8_t             rxCharBuffer;
} usartHandle;

/* Implement USART device */
bool usartInit(usartHandle *handle, USART_TypeDef *usartInstance, uint32_t baudRate);

/*  Enable printf on USART handler.
    Note that it will work only one usart interface
*/
void usartEnableDebug(usartHandle *handle);

#endif
