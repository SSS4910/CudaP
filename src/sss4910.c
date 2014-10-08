#include <stdio.h>
#include <pthread.h>

#include "accessreader.h"
#include "cudaparser.h"

struct Node {
    char* line; 
    struct Node* next;
    struct Node* prev;
};

struct Queue {
    struct Node* head;
    struct Node* tail;
};

int push_to_queue(struct Node *);
int pop_off_queue(struct Node *);

int
main(int argc, char** argv){
    int err;
    pthread_t accessReader;
    pthread_t cudaParser;

    printf("Be Brave!\n");
    //handle options and flags (getopt)

    //declare thread-safe Queue

    //declare AccessReader thread
    err = pthread_create(&accessReader, NULL, access_reader,NULL);
    if (err !=0)
    {
        printf("error1: couldn't create AccessReader thread!!!\n");
        return 1;
    }
    //declare CudaParser thread
    err = pthread_create(&cudaParser, NULL, cuda_parser, NULL);
    if (err != 0)
    {
        printf("error2: couldn't create CudaParser thread!!!\n");
        return 1;
    }
    //initiate AccessReader thread

    //initiate CudaParser thread
    
    //join AccessReader thread
    pthread_join(accessReader, NULL);
    //join CudaParser thread
    pthread_join(cudaParser, NULL);
    //cleanup
    return 0;
}

/*
 * Adds node structure to tail of queue
 * Returns 0 if successful
 */

int
push_to_queue(struct Node* nodeToAdd){
    return 0;
}


/*
 * Remove node structure from head of queue
 * Returns 0 if successful
 */
int
pop_off_queue(struct Node* node){
    return 0;
}
