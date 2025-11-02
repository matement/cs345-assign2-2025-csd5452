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


extern pthread_mutex_t tlck;
extern pthread_mutex_t glck;
extern pthread_mutex_t general;

typedef struct{
    int people;
    int id;
    int seated; //0 if waiting 1 if eating
    int timespent;
}Group;

typedef struct{
    int capacity;
    int id;
    int seatsAvailabe; 
}Table;

typedef struct{
    Group *g;
    Table *t;
}Pair;

#include "queue.h"

Group* initgoups(int ngroups, int tablecap);
Table* innittables(int tablecap, int tablenum);

void start(Group *groups, Table *tables, pthread_t waiter, pthread_t *gtherads, int ngroups, int tablenum);

#endif