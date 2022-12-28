#ifndef __FIFO_H__
#define __FIFO_H__

#include <stdbool.h>
#include <stdint.h>
#include "config.h"


typedef struct {
    uint8_t            content[FIFO_BUFFER_SIZE];
    uint8_t           *start;
    uint8_t           *end;
} fifoData;

void fifoInit(fifoData *fifo);
void fifoPush(fifoData *fifo, uint8_t Data);
uint8_t fifoPop(fifoData *fifo);

bool fifoIsEmpty(fifoData *fifo);
bool fifoIsFull(fifoData *fifo);


#endif
