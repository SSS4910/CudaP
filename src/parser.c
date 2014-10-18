#include <stdio.h>
#include <pthread.h>

#include "parser.h"
#include "debug.h"
#include "sss4910.h"

#define MAX_THREADS 512

void *
process_buffer(void* arg){
    int i;
    Buffer * buffer = (Buffer *) arg;
    pthread_t parseThreads[MAX_THREADS];

    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_create(&parseThreads[i], NULL, parse_line, (void *) buffer->strArray[i]);
    }

    for (i = 0; i < MAX_THREADS; i++)
    {
        pthread_join(parseThreads[i], NULL);
    }
    buffer->available = TRUE;
    return 0;
}

void *
parse_line(void * arg){
    char * line = (char *) arg;
    printf("%s", line);
    return 0;
}
