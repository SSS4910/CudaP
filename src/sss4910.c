#include <stdio.h>

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
    printf("Be Brave!\n");
    //handle options and flags (getopt)

    //declare thread-safe Queue

    //declare AccessReader thread

    //declare CudaParser thread
    
    //initiate AccessReader thread

    //initiate CudaParser thread
    
    //join AccessReader thread
    
    //join CudaParser thread

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
