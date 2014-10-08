#include <stdio.h>
#include <pthread.h>

#include "cudaparser.h"

void* cuda_parser(void* arg){
    printf("In Cuda Parser!!!\n");
    pthread_exit(NULL);
}
