#include "assign2.h"
pthread_mutex_t tlck;
pthread_mutex_t glck;
pthread_mutex_t general;

int main(int argc, char **argv){
    if(argc != 4 || atoi(argv[1]) < 0 || atoi(argv[2])<0 || atoi(argv[3])<0){
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }
    else{
        srand(time(NULL));
        int ngroups = atoi(argv[2]);
        int tablecap = atoi(argv[3]);
        int tablenum = atoi(argv[1]);
        
        Group *groups = initgoups(ngroups, tablecap);
        Table *tables = innittables(tablecap, tablenum);
        pthread_mutex_init(&tlck, NULL);
        pthread_mutex_init(&glck, NULL);
        pthread_mutex_init(&general, NULL);

        pthread_t waiter;
        pthread_t *gtherads = malloc(ngroups * sizeof(pthread_t));

        start(groups, tables, waiter, gtherads, ngroups, tablenum);
        pthread_mutex_destroy(&tlck);
    }
}