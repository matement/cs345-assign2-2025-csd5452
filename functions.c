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
    printf("Eating(%d people) for %d seconds...)", g.people, g.timespent);
    sleep(g.timespent);
}

void* assignSeats(void* args) {
    Pair *p = (Pair *) args;

    Table *t = p->t; 
    Group *g = p->g;  

    pthread_mutex_lock(&tlck);
    printf("In da mutex\n");

    if ((t->seatsAvailabe - g->people) >= 0) {
        t->seatsAvailabe -= g->people; 
        printf("[%s]Assigned group %d (size=%d) to Table %d (%d/%d occupied)\n", p->role ,g->id, g->people, t->id, t->capacity - t->seatsAvailabe, t->capacity);
    }

    pthread_mutex_unlock(&tlck);

    free(p);
    return NULL;
}

Table* findTable(Table *t, int tablenum, int people){
    for(int i = 0; i<tablenum; i++){
        if(people<tablenum){
            return &t[i];
            
        }
    }
    return NULL;
}

void* arival(void* arg){
    Pair *p = (Pair *)arg;
    Group *g = p->g;
    printf("[Group %d]Arived with %d people(after %d sec)\n",g->id, g->people, g->timespent);
    return NULL;
}

void start(Group *groups, Table *tables, pthread_t waiter, pthread_t *gtherads, int ngroups, int tablenum){
    Node *head = NULL;
    
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
        Table *t = findTable(tables, tablenum, groups[r].people);

        Pair* p0 = malloc(sizeof(Pair));
        p0->g = &groups[r];
        p0->t = NULL;
        p0->role = NULL;

        pthread_create(&gtherads[r], NULL, &arival, p0);

        Pair *p1 = malloc(sizeof(Pair));
        
        p1->g = &groups[r];
        p1->t = &t;
        p1->role = "Waiter";
        
        pthread_create(&waiter, NULL, &assignSeats, p1);
        
    }
    
    free(head);
}