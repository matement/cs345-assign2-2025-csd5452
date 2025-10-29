#include "assign2.h"

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

void eat(Group g){
    printf("Eating(%d people) for %d seconds...", g.people, g.timespent);
    sleep(g.timespent);
}

void* assignSeats(void* args) {
    Pair *p = (Pair *) args;

    Table *t = &p->t;  // pointer to actual table
    Group *g = &p->g;  // pointer to actual group

    if ((t->seatsAvailabe - g->people) >= 0) {
        t->seatsAvailabe -= g->people; // subtract directly
        printf("Assigned group %d (size=%d) to Table %d (%d/%d occupied)\n",
               g->id, g->people, t->id,
               t->capacity - t->seatsAvailabe, t->capacity);
    }

    free(p);
    return NULL;
}


void start(Group *groups, Table *tables, pthread_t waiter, pthread_t *gtherads, int ngroups){
    int groupNumbers[ngroups];
    for(int i = 0; i<ngroups; i++){
        groupNumbers[i] = i;
    }
    int r = rand()%ngroups;
    Pair *p1 = malloc(sizeof(Pair));
    p1->g = groups[r];
    p1->t = tables[0];
    waiter = pthread_create(&waiter, NULL, &assignSeats, p1);
    pthread_exit(NULL);
    free(p1);
}