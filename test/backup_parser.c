#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "parser.h"
#include "debug.h"
#include "sss4910.h"

void *
process_buffer(void* arg){
    int i;
    void * status;
    Buffer * buffer = (Buffer *) arg;

    /*
    for (i = 0; i < MAX_THREADS; i++)
    {
        parse_line((void *) buffer->strArray[i]);
    }
    */

    pthread_t parseThreads[buffer->currentSize];

    for (i = 0; i < buffer->currentSize; i++)
    {
        pthread_create(&parseThreads[i], NULL, parse_line, (void *) buffer->strArray[i]);
    }

    for (i = 0; i < buffer->currentSize; i++)
    {
        strncpy(buffer->strArray[i], "***", MAX_LINE_LENGTH);
        pthread_join(parseThreads[i], &status);
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
