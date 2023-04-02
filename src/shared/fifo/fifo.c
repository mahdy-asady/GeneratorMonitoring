#include "fifo.h"


//*************** 8 bit fifo functions *********************************************************
void fifo8Init(fifo8Data *fifo) {
    fifo->start = fifo->end = fifo->content;
}

uint8_t *_Next8bitIndex(fifo8Data *fifo, uint8_t *Index) {
    if((Index + 1) == (fifo->content + FIFO_BUFFER_SIZE))
        return fifo->content;
    else
        return Index + 1;
}

void fifo8Push(fifo8Data *fifo, uint8_t Data) {
    *(fifo->end) = Data;
    fifo->end = _Next8bitIndex(fifo, fifo->end);
}

uint8_t fifo8Pop(fifo8Data *fifo) {
    uint8_t Data = *(fifo->start);
    fifo->start = _Next8bitIndex(fifo, fifo->start);
    return Data;
}


bool fifo8IsEmpty(fifo8Data *fifo) {
    return fifo->start == fifo->end;
}

bool fifo8IsFull(fifo8Data *fifo) {
    return fifo->start == _Next8bitIndex(fifo, fifo->end);
}



//*************** 32 bit fifo functions ********************************************************

void fifo32Init(fifo32Data *fifo) {
    fifo->start = fifo->end = fifo->content;
}

uint32_t *_Next32bitIndex(fifo32Data *fifo, uint32_t *Index) {
    if((Index + 1) == (fifo->content + FIFO_BUFFER_SIZE))
        return fifo->content;
    else
        return Index + 1;
}

void fifo32Push(fifo32Data *fifo, uint32_t Data) {
    *(fifo->end) = Data;
    fifo->end = _Next32bitIndex(fifo, fifo->end);
}

uint32_t fifo32Pop(fifo32Data *fifo) {
    uint32_t Data = *(fifo->start);
    fifo->start = _Next32bitIndex(fifo, fifo->start);
    return Data;
}


bool fifo32IsEmpty(fifo32Data *fifo) {
    return fifo->start == fifo->end;
}

bool fifo32IsFull(fifo32Data *fifo) {
    return fifo->start == _Next32bitIndex(fifo, fifo->end);
}
