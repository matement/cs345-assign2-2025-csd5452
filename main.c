#include "assign2.h"

int main(int argc, char **argv){
    if(argc != 4 || atoi(argv[1]) < 0 || atoi(argv[2])<0 || atoi(argv[3])<0){
        perror("Invalid input\n");
        exit(-1);
    }
    else{
        int ngroups = atoi(argv[1]);
        int tablecap = atoi(argv[2]);
        Group_t *groups = malloc(sizeof(ngroups));

        for(int i = 0; i<ngroups; i++){
            groups[i].id = i;

        }
    }
}