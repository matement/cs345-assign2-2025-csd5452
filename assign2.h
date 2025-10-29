#ifndef __ASSIGN_2__
#define __ASSIGN_2__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>
#include <time.h>


typedef struct Group{
    int people;
    int id;
}Group_t;

extern pthread_t waiter;
extern pthread_t *group;

#endif