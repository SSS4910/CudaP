#include <stdio.h>
#include <pthread.h>

#include "cudaparser.h"
#include "debug.h"

void* cuda_parser(void* arg){
    errlog_write("Initiating Cuda Parser thread\n");
    errlog_write("Exiting Cuda Parser thread\n");
    pthread_exit(NULL);
}
