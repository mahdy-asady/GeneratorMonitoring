#include "fifo.h"

void fifoInit(fifoData *fifo) {
    fifo->start = fifo->end = fifo->content;
}

uint8_t *_NextIndex(fifoData *fifo, uint8_t *Index) {
    if((Index + 1) == (fifo->content + FIFO_BUFFER_SIZE))
        return fifo->content;
    else
        return Index + 1;
}

void fifoPush(fifoData *fifo, uint8_t Data) {
    *(fifo->end) = Data;
    fifo->end = _NextIndex(fifo, fifo->end);
}

uint8_t fifoPop(fifoData *fifo) {
    uint8_t Data = *(fifo->start);
    fifo->start = _NextIndex(fifo, fifo->start);
    return Data;
}


bool fifoIsEmpty(fifoData *fifo) {
    return fifo->start == fifo->end;
}

bool fifoIsFull(fifoData *fifo) {
    return fifo->start == _NextIndex(fifo, fifo->end);
}