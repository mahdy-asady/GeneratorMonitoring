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


void usartWrite(usartHandle *handle, uint8_t *Data, uint16_t Size);

void usartWriteLine(usartHandle *handle, char *Text);

uint16_t usartRead(usartHandle *handle, uint8_t *Buffer, uint16_t MaxSize, uint16_t Timeout);

uint16_t usartReadLine(usartHandle *handle, char *Buffer, uint16_t MaxSize, uint16_t Timeout);

#endif
