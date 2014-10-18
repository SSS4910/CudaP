/*
 * TODO:
 *  -> BUG #1 - need to properly cleanup if buffer reading fails
 *  -> BUG #2 -
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

Buffer *
create_buffer();

int
fill_buffer(Buffer *, FILE *);

int
main(int argc, char** argv){
    int err;
    FILE * logfile;
    Buffer * leftBuffer;
    Buffer * rightBuffer;
    pthread_t processThreadLeft;
    pthread_t processThreadRight;

    open_error_logfile();
    //handle options and flags (getopt)

    errlog_write("Initiating Access Reader thread\n");
    errlog_write("Opening access.log file\n");
    if ((logfile = fopen("access.log", "r")) == NULL)
    {
        errlog_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }

    errlog_write("Allocating memory for left buffer\n");
    errlog_write("Allocating memory for right buffer\n");
    //at some point, we need to make this larger than 4096
    //its a pretty big deal
    
    leftBuffer = create_buffer();
    rightBuffer = create_buffer();

    leftBuffer->available = TRUE;
    rightBuffer->available = TRUE;

    while (!feof(logfile))
    {
        if (leftBuffer->available)
        {
            leftBuffer->available = FALSE;
            err = fill_buffer(leftBuffer, logfile);
            if (err == -1)
            {
                //fix this later -> see BUG #1
                return -1;
            }

            err = pthread_create(&processThreadLeft, NULL, process_buffer, (void *) leftBuffer);
            if (err != 0)
            {
                //fix this later
                return -1;
            }
        }

        if (rightBuffer->available)
        {
            rightBuffer->available = FALSE;
            err = fill_buffer(rightBuffer, logfile);
            if (err == -1)
            {
                //fix this later
                return -1;
            }
            
            err = pthread_create(&processThreadRight, NULL, process_buffer, (void *) rightBuffer);
            if (err != 0)
            {
                //fix this later
                return -1;
            }
        }
    }
    //cleanup
    
    pthread_join(processThreadLeft, NULL);
    pthread_join(processThreadRight, NULL);
    errlog_write("Closing access.log file...\n");
    fclose(logfile);

    free(leftBuffer);
    free(rightBuffer);

    errlog_write("Freeing memory for line buffer\n");
 
    close_error_logfile();
    return 0;
}

Buffer * 
create_buffer(){
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

int
fill_buffer(Buffer * buffer, FILE * logfile){
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (fgets(buffer->strArray[i], MAX_LINE_LENGTH, logfile) == NULL)
        {
            return -1;
        }
    }
    return 0;
}
