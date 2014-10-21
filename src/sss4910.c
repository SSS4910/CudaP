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

char *
log_readline(FILE *);

int
main(int argc, char** argv){
    int i;
    //int err;
    char * logline;
    FILE * logfile;
    Buffer * buffer;

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
    
    buffer = create_buffer();
    buffer->available = TRUE;

    while (!feof(logfile))
    {
        buffer->currentSize = 0;
        for (i = 0; i < BUFFER_SIZE; i++)
        {
            //read a line
            logline = log_readline(logfile);
            if ((logline == (char *) NULL) && feof(logfile))
            {
                printf("hit eof\n");
            }

            //parse a line
            //LogLine = parse(logline);

            //add LogLine to buffer
            buffer->strArray[i] = logline;
            buffer->currentSize++;
        }
    }

    //cleanup
    errlog_write("Closing access.log file...\n");
    fclose(logfile);

    free(buffer);
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

char *
log_readline(FILE * logfile){
    char * line = malloc((MAX_LINE_LENGTH) * sizeof(char));
    if (fgets(line, MAX_LINE_LENGTH, logfile) != NULL)
    {
        return line;
    }
    return (char *)NULL;
}
