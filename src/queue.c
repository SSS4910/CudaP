/*
    queue.c
    by: Michael Shields
    10-9-2014

    Description:
    ...
*/

# include <stdio.h>
# include <string.h>
# include "queue.h"


char * 
queue_read(Queue *queue){
    char * line = "";
    // Lock?
    if(is_queue_ready_read(queue) == 1)
    {
        line = strcpy(line, queue->strArray[queue->readPtr]);
        queue->strArray[queue->readPtr] = strcpy(queue->strArray[queue->readPtr], "***"); // tombstone after read
        queue->readPtr++;
        if(queue->readPtr >= queue->MAX_SIZE) // wrap-around
        {
            queue->readPtr = 0;
        }
    }
    else // queue is not ready
    {
        line = strcpy(line, "NULL");
    }

    return line;
}

int
queue_write(Queue *queue, char *strToWrite){
    int success = 0;
    if(is_queue_ready_write(queue) == 1)
    {
        strcpy(queue->strArray[queue->writePtr], strToWrite);
        queue->writePtr++; 
        if(queue->writePtr >= queue->MAX_SIZE) // wrap-around
        {
            queue->writePtr = 0;
        }

        success = 1;
    }

    return success;
}

int// or int??
is_queue_ready_write(Queue *queue){
    int ready = 1;

    if(queue->readPtr == queue->writePtr) // pointers on top of each other
    {
        ready = 0;
    }
    else if(strcmp(queue->strArray[queue->writePtr], "***") != 0) // location is NOT tombstoned
    {
        ready = 0;
        // loop until string == *** ??
    }

    return ready;
}

int// or int??
is_queue_ready_read(Queue *queue){
    int ready = 1;

    if(queue->readPtr == queue->writePtr) // pointers on top of each other
    {
        ready = 0;
    }
    else if(strcmp(queue->strArray[queue->readPtr], "***")) // location is tombstoned
    {
        ready = 0;
        // loop until string != *** ??
    }

    return ready;

}

Queue *
queue_init(Queue *queue){

    queue->writePtr = 0;
    queue->readPtr = 0;
    queue->MAX_SIZE = 20;
    queue->strArray[queue->MAX_SIZE];

    return queue;
}

int 
queue_teardown(Queue *queue){
    return 1;
}
