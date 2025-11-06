#include "assign2.h"
pthread_mutex_t tlck;
pthread_mutex_t glck;
pthread_mutex_t general;
sem_t groupsSem;
sem_t tablesSem;


int main(int argc, char **argv){
    if(argc != 4 || atoi(argv[1]) < 0 || atoi(argv[2])<0 || atoi(argv[3])<0){
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    else{
        srand(time(NULL));
        /*put data in vars*/
        int ngroups = atoi(argv[2]);
        int tablecap = atoi(argv[3]);
        int tablenum = atoi(argv[1]);

        /*initialise*/
        Group *groups = initgoups(ngroups, tablecap);
        Table *tables = innittables(tablecap, tablenum);
        pthread_mutex_init(&tlck, NULL);
        pthread_mutex_init(&glck, NULL);
        pthread_mutex_init(&general, NULL);
        sem_init(&groupsSem, 0, 0);
        sem_init(&tablesSem, 0, tablenum);
        for (int i = 0; i < ngroups; i++) {
            sem_init(&groups[i].seatedsem, 0, 0);
            groups[i].assignedTable = NULL;
        }

        /*hanle the waiter thread*/
        pthread_t waiter;
        pthread_t *gthreads = malloc(ngroups * sizeof(pthread_t));

        Node *head = NULL;
        Queue *waitQueue = malloc(sizeof(Queue));
        Qinit(waitQueue, ngroups);
        //QprintQueue(waitQueue);
        int i=0;


        waiterArgs *wa = malloc(sizeof(waiterArgs));
        volatile int done = 0;
        wa->tables = tables;
        wa->waitQueue = waitQueue;
        wa->groupnum = ngroups;
        wa->tablenum = tablenum;
        wa->groups = groups;
        wa->done = &done;

        pthread_create(&waiter, NULL, &WaiterFunctions, wa);
        
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

            Pair *pg = innitPair(&groups[r], NULL, waitQueue);

            pthread_create(&gthreads[r], NULL, &GroupsFunctions, pg);
            

        }
        
        for(int i = 0; i<ngroups; i++){
            pthread_join(gthreads[i], NULL);
            
        }
        done = 1;
        sem_post(&groupsSem);
        pthread_join(waiter, NULL);
        free(head);

        for (int i = 0; i < ngroups; i++)
            sem_destroy(&groups[i].seatedsem);

        sem_destroy(&groupsSem);
        sem_destroy(&tablesSem);
        pthread_mutex_destroy(&tlck);

        printf("All groups have left. Closing restaurant.\n");
    }
}