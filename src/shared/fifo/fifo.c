#include "fifo.h"

void fifo8Init(fifo8Data *fifo) {
    fifo->start = fifo->end = fifo->content;
}

uint8_t *_NextIndex(fifo8Data *fifo, uint8_t *Index) {
    if((Index + 1) == (fifo->content + FIFO_BUFFER_SIZE))
        return fifo->content;
    else
        return Index + 1;
}

void fifo8Push(fifo8Data *fifo, uint8_t Data) {
    *(fifo->end) = Data;
    fifo->end = _NextIndex(fifo, fifo->end);
}

uint8_t fifo8Pop(fifo8Data *fifo) {
    uint8_t Data = *(fifo->start);
    fifo->start = _NextIndex(fifo, fifo->start);
    return Data;
}


bool fifo8IsEmpty(fifo8Data *fifo) {
    return fifo->start == fifo->end;
}

bool fifo8IsFull(fifo8Data *fifo) {
    return fifo->start == _NextIndex(fifo, fifo->end);
}