#include <stdio.h>
#include <pthread.h>

#include "accessreader.h"

void* access_reader(void* arg){
    printf("In Access Reader!!!\n");
    pthread_exit(NULL);
}
