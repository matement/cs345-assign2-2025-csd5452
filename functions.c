#include "assign2.h"

Pair* innitPair(Group *g, Table* t){
    Pair *p = malloc(sizeof(Pair));
    p->g = g;
    p->t = t;
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
        tables[i].seatsAvailabe = tablecap;
    }
    return tables;
}


void* eat(void* args){
    Pair* p = (Pair*)args;
    Group* g = p->g;
    printf("[Group %d]Eating(%d people) for %d seconds...\n",g->id, g->people, g->timespent);
    //sleep(g->timespent);
    return NULL;
}

void* assignSeats(void* args) {
    Pair *p = (Pair *) args;

    Table *t = p->t; 
    Group *g = p->g;  

    pthread_mutex_lock(&tlck);

    if ((t->seatsAvailabe - g->people) >= 0) {
        t->seatsAvailabe -= g->people; 
        printf("[Waiter]Assigned group %d (size=%d) to Table %d (%d/%d occupied)\n", g->id, g->people, t->id, t->capacity - t->seatsAvailabe, t->capacity);
    }

    pthread_mutex_unlock(&tlck);

    return NULL;
}

Table* findTable(Table *t, int tablenum, int people){
    for(int i = 0; i<tablenum; i++){
        if(people<=t[i].seatsAvailabe){
            return &t[i];
            
        }
    }
    return NULL;
}

void* arival(void* arg){
    Pair *p = (Pair *)arg;
    Group *g = p->g;
    int r = rand()%15;
    pthread_mutex_lock(&glck);
    
    printf("[Group %d]Arived with %d people(after %d sec)\n",g->id, g->people, r);
    //sleep(r);
    pthread_mutex_unlock(&glck);
    return NULL;
}

void* waiting(void* args){
    Queue* q = (Queue* )args;
    pthread_mutex_lock(&glck);
    printf("[Waiter] Groups waiting: %d\n", q->size);
    pthread_mutex_unlock(&glck);
    return NULL;    
}

void* leaveTable(void* args){
    Pair* p = (Pair*)args;
    Group* g = p->g;
    Table* t = p->t;
    pthread_mutex_lock(&glck);
    
    printf("[Group %d] left table %d(%d/%d Occupied)\n", g->id, t->id, t->seatsAvailabe, t->capacity);
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

void start(Group *groups, Table *tables, pthread_t waiter, pthread_t *gthreads, int ngroups, int tablenum){
    Node *head = NULL;
    Queue *WaitQueue = malloc(sizeof(Queue));
    Qinit(WaitQueue, ngroups);
    QprintQueue(WaitQueue);
    int i=0;
    while(i<ngroups){
        append(&head, i);
        i++;
    }
    printList(head);
    while(head != NULL){
        int r = rand()%ngroups;
        
        if(!nodeExists(head, r)){ 
            continue;
        }
        delete_node(&head, r);
        

        Table *t = findTable(tables, tablenum, groups[r].people);
        if(t == NULL){ 
            Qenqueue(WaitQueue, groups[r]);
            continue;
        }

        Pair* p0 = innitPair(&groups[r], NULL);

        pthread_create(&gthreads[r], NULL, &arival, p0);
                

        Pair *p1 = innitPair(&groups[r], t);

        
        pthread_create(&waiter, NULL, &assignSeats, p1);

        Pair* p3 = innitPair(&groups[r], NULL);
        
        pthread_create(&gthreads[r], NULL, &eat, p3);

        pthread_create(&waiter, NULL, &waiting, WaitQueue);

        pthread_create(&gthreads[r], NULL, &leaveTable, p1);
        
        pthread_create(&gthreads[r], NULL, &seated, p1);
    }
    
    for(int i = 0; i<=ngroups; i++){
        pthread_join(gthreads[i], NULL);
    }

        pthread_join(waiter, NULL);

    free(head);
}

/**
 * kane ola ta goub threads se ena function kai ta waiter thread se ena allo.
 * des an mporeis na kaneis thn lista se array gia cleaner handling
 * prospathise na valeis semafores 
 * gamas
 */