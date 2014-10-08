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

int
main(int argc, char** argv){
    struct Queue queue;
    struct Node node;

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
