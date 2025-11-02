#ifndef QUEUE_H
#define QUEUE_H

#include "assign2.h"

typedef struct {
    Group *data;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

void Qinit(Queue *q, int capacity);
int QisFull(Queue *q);
int QisEmpty(Queue *q);
void Qenqueue(Queue *q, Group value);
Group Qdequeue(Queue *q);
void QprintQueue(Queue *q);
void Qdestroy(Queue *q);
int Qcount();

#endif
