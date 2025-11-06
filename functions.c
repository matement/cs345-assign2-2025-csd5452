#include "assign2.h"

Pair* innitPair(Group *g, Table* t, Queue *q){
    Pair *p = malloc(sizeof(Pair));
    p->g = g;
    p->t = t;
    p->q = q;
    return p;
}

Group* initgoups(int ngroups, int tablecap){
    Group *groups = malloc(ngroups*sizeof(Group));
    if(!groups){
        fprintf(stderr, "Couldnt allocate groups\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i<ngroups; i++){
        groups[i].id = i;
        groups[i].people = (rand()%tablecap)+1;
        groups[i].seated = 0;
        groups[i].timespent = (rand() % (15 - 5 + 1)) + 5;
        groups[i].assignedTable = NULL;
    }
    return groups;
}

Table* innittables(int tablecap, int tablenum){
    Table* tables = malloc(tablenum * sizeof(Table));
    if(!tables){
        fprintf(stderr, "Coulndt allocate tables\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i<tablenum; i++){
        tables[i].capacity = tablecap;
        tables[i].id = i;
        tables[i].seatsAvailable = tablecap;
    }
    return tables;
}

void* WaiterFunctions(void* args){
    waiterArgs *wa = (waiterArgs*)args;
    Table *tables = wa->tables;
    Queue *waitQueue = wa->waitQueue;
    Group *groups = wa->groups;
    volatile int *done = wa->done;
    while(1){

        sem_wait(&groupsSem);

        if(*done) break;

        sem_wait(&tablesSem);

        pthread_mutex_lock(&tlck);
        
        Group *g =  Qdequeue(waitQueue);

        Table* t = findTable(tables, wa->tablenum, g->people);
        if(t == NULL){
            Qenqueue(waitQueue, g);
            pthread_mutex_unlock(&tlck);

            sem_post(&groupsSem);
            sem_post(&tablesSem);
            continue;
        }
                
        t->seatsAvailable -= g->people;
        g->assignedTable = t;

       
        printf("[Waiter]Assigned group %d (size=%d) to Table %d (%d/%d occupied)\n", g->id, g->people, t->id, t->capacity - t->seatsAvailable, t->capacity);
        

        printf("[Waiter] Groups waiting: %d\n", waitQueue->size);

        pthread_mutex_unlock(&tlck);

        
        sem_post(&g->seatedsem);

        
        continue;
    }
    return NULL;
}

void* GroupsFunctions(void* args){
    Pair *p = (Pair*)args;
    Queue* q = p->q;
    Group* g = p->g;

    arival(p);

    pthread_mutex_lock(&tlck);
    Qenqueue(q, g);
    pthread_mutex_unlock(&tlck);

    sem_post(&groupsSem);

    
    sem_wait(&g->seatedsem);

    p->t = g->assignedTable;

    seated(p);
    eat(p);
    leaveTable(p);

    sem_post(&tablesSem);
    free(p);
    return NULL;
}


void* eat(void* args){
    Pair* p = (Pair*)args;
    Group* g = p->g;
    pthread_mutex_lock(&tlck);
    printf("[Group %d] Eating(%d people) for %d seconds...\n",g->id, g->people, g->timespent);
    sleep(g->timespent);
    pthread_mutex_unlock(&tlck);
    return NULL;
}


Table* findTable(Table *t, int tablenum, int people){
    for(int i = 0; i<tablenum; i++){
        if(people<=t[i].seatsAvailable){
            return &t[i];
            
        }
    }
    return NULL;
}

void* arival(void* arg){
    Pair *p = (Pair *)arg;
    Group *g = p->g;
    int r = rand()%5;
    pthread_mutex_lock(&glck);
    
    printf("[Group %d]Arived with %d people(after %d sec)\n",g->id, g->people, r);
    sleep(r);
    pthread_mutex_unlock(&glck);
    return NULL;
}



void* leaveTable(void* args){
    Pair* p = (Pair*)args;
    Group* g = p->g;
    Table* t = p->t;
    pthread_mutex_lock(&glck);
    t->seatsAvailable += g->people;  
    g->assignedTable = NULL;
    printf("[Group %d] left table %d(%d/%d Occupied)\n", g->id, t->id, t->capacity-t->seatsAvailable, t->capacity);
    pthread_mutex_unlock(&glck);
    return NULL;
}

void* seated(void* args){
    Pair* p = (Pair*)args;
    Group* g = p->g;
    Table* t = p->t;

    pthread_mutex_lock(&tlck);
    printf("[Group %d] Seated at table %d with %d people\n", g->id, t->id, g->people);
    pthread_mutex_unlock(&tlck);
    return NULL;
}

