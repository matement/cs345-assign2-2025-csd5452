#include "queue.h"

void Qinit(Queue *q, int capacity) {
    q->data = malloc(sizeof(Group) * capacity);
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

void Qenqueue(Queue *q, Group value) {
    if (QisFull(q)) return;
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = value;
    q->size++;
}

Group Qdequeue(Queue *q) {
    Group dummy = {0, 0, 0, 0};
    if (QisEmpty(q)) return dummy;
    Group value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

void QprintQueue(Queue *q) {
    if (QisEmpty(q)) return;
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->capacity;
        Group g = q->data[index];
        printf("Group %d: people=%d, seated=%d, timespent=%d\n", g.id, g.people, g.seated, g.timespent);
    }
}

void Qdestroy(Queue *q) {
    free(q->data);
}

