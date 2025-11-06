#ifndef __ASSIGN_2__
#define __ASSIGN_2__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "list.h"

typedef struct Queue Queue;

extern pthread_mutex_t tlck;
extern pthread_mutex_t glck;
extern pthread_mutex_t general;

extern sem_t groupsSem;
extern sem_t tablesSem;


typedef struct{
    int capacity;
    int id;
    int seatsAvailable; 
}Table; 

typedef struct{
    int people;
    int id;
    int seated; //0 if waiting 1 if eating
    int timespent;
    sem_t seatedsem;
    Table *assignedTable;
}Group;



#include "queue.h"

typedef struct{
    Group *g;
    Table *t;
    Queue *q;
}Pair;

typedef struct{
    Table *tables;
    int tablenum;
    Queue *waitQueue;
    Group *groups;
    int groupnum;
    volatile int* done;
}waiterArgs;

Group* initgoups(int ngroups, int tablecap);
Table* innittables(int tablecap, int tablenum);
void* assignSeats(void *args);
void* waiting(void *args);
void* arival(void *args);
void* eat(void *args);
void* seated(void *args);
void* leaveTable(void *args);
void* WaiterFunctions(void *args);
void* GroupsFunctions(void *args);
Pair* innitPair(Group *g, Table* t, Queue *q);
Table* findTable(Table *t, int tablenum, int people);

void start(Group *groups, Table *tables, pthread_t waiter, pthread_t *gtherads, int ngroups, int tablenum);

#endif