#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdbool.h>
#include <stdint.h>
#include "config.h"


typedef struct {
    uint8_t            content[FIFO_BUFFER_SIZE];
    uint8_t           *start;
    uint8_t           *end;
} fifo8Data;

void fifo8Init(fifo8Data *fifo);
void fifo8Push(fifo8Data *fifo, uint8_t Data);
uint8_t fifo8Pop(fifo8Data *fifo);

bool fifo8IsEmpty(fifo8Data *fifo);
bool fifo8IsFull(fifo8Data *fifo);


#endif
