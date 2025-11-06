#include "queue.h"

void Qinit(Queue *q, int capacity){
    q->data = malloc(sizeof(Group*) * capacity);
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->capacity = capacity;
}

int QisFull(Queue *q) {
    return q->size == q->capacity;
}

int QisEmpty(Queue *q) {
    return q->size == 0;
}

void Qenqueue(Queue *q, Group *g){
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = g;   // store pointer, NOT copy
    q->size++;
}

Group *Qdequeue(Queue *q){
    Group *g = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return g;
}


void Qdestroy(Queue *q) {
    free(q->data);
}

