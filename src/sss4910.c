#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "accessreader.h"
#include "cudaparser.h"
#include "debug.h"

#define BUFFER_SIZE 512
#define MAX_LINE_LENGTH 4096  

typedef struct{
    int available;
    char ** strArray; 
    struct Node* next;
} Buffer;

Buffer * leftBuffer;
Buffer * rightBuffer;

Buffer *
create_buffer();

int
main(int argc, char** argv){
    int err;
    FILE * logfile;
    pthread_t cudaParser;

    open_error_logfile();
    //handle options and flags (getopt)

    errlog_write("Initiating Access Reader thread\n");
    errlog_write("Opening access.log file\n");
    if ((logfile = fopen("access.log", "r")) == NULL)
    {
        return -1;
    }

    errlog_write("Allocating memory for line buffer\n");
    //at some point, we need to make this larger than 4096
    //its a pretty big deal
    
    leftBuffer = create_buffer();
    rightBuffer = create_buffer();

    leftBuffer->available = 1;
    rightBuffer->available = 1;

    while (!feof(logfile))
    {
        if (leftBuffer->available)
        {
            int i;
            for (i = 0; i < BUFFER_SIZE; i++)
            {
                if (fgets(leftBuffer->strArray[i], MAX_LINE_LENGTH, logfile) != NULL)
                {
                    printf("%s", leftBuffer->strArray[i]);
                }
            }
        }

        if (rightBuffer->available)
        {
            
            printf("YEAHHHHHHHHHHHHHHH BUUUUUUUDDDDY\n");
        }
    }

    //Initialize cudaParser thread
    err = pthread_create(&cudaParser, NULL, cuda_parser, NULL);
    if (err != 0)
    {
        printf("error2: couldn't create CudaParser thread!!!\n");
        return 1;
    }   
    //join CudaParser thread
    pthread_join(cudaParser, NULL);

    //cleanup
    errlog_write("Closing access.log file...\n");
    fclose(logfile);

    free(leftBuffer);
    free(rightBuffer);

    errlog_write("Freeing memory for line buffer\n");
 
    close_error_logfile();
    return 0;
}

Buffer * create_buffer(){
    Buffer * buffer;
 
    buffer = malloc(sizeof(Buffer));
    if (buffer == NULL)
    {
        return (Buffer * ) NULL;
    }

    buffer->strArray = malloc (BUFFER_SIZE * sizeof (char*));
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        buffer->strArray[i] = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
    }

    return buffer;
}
