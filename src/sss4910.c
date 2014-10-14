#include <stdio.h>
#include <pthread.h>

#include "accessreader.h"
#include "cudaparser.h"
#include "debug.h"

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
    int err;
    pthread_t accessReader;
    pthread_t cudaParser;

    open_error_logfile();
    //handle options and flags (getopt)

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
    close_error_logfile();
    return 0;
}


