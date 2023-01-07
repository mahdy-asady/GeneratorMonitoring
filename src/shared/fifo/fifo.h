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


typedef struct {
    uint32_t            content[FIFO_BUFFER_SIZE];
    uint32_t           *start;
    uint32_t           *end;
} fifo32Data;

void fifo8Init(fifo8Data *fifo);
void fifo8Push(fifo8Data *fifo, uint8_t Data);
uint8_t fifo8Pop(fifo8Data *fifo);

bool fifo8IsEmpty(fifo8Data *fifo);
bool fifo8IsFull(fifo8Data *fifo);


void fifo32Init(fifo32Data *fifo);
void fifo32Push(fifo32Data *fifo, uint32_t Data);
uint32_t fifo32Pop(fifo32Data *fifo);

bool fifo32IsEmpty(fifo32Data *fifo);
bool fifo32IsFull(fifo32Data *fifo);

#endif
