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

/*
 * FUNCTION: main
 * --------------
 * the main entry point of the application
 *
 * argc: the number of command line arguments
 * argv: the command line arguments, kept as a pointer to a string
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
main(int argc, char** argv){
    int i;
    //int err;
    char * logline;
    FILE * logfile;
    Buffer * buffer;

    open_debug_file();
    //handle options and flags (getopt)

    debug_write("Initiating Access Reader thread\n");
    debug_write("Opening access.log file\n");
    if ((logfile = fopen("access.log", "r")) == NULL)
    {
        debug_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }

    debug_write("Allocating memory for left buffer\n");
    debug_write("Allocating memory for right buffer\n");
    
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
    debug_write("Closing access.log file...\n");
    fclose(logfile);

    free(buffer);
    debug_write("Freeing memory for line buffer\n");
 
    close_debug_file();
    return 0;
}


/*
 * FUNCTION: create_buffer
 * -----------------------
 * constructs a buffer to store each line of the log
 *
 * return: a pointer to a buffer structure on success, NULL on failure
 */
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

/*
 * FUNCTION: log_readline
 * ----------------------
 * reads next line from provided log file
 *
 * logfile: the log file pointer
 *
 * return: a newline terminated string containing the next line from file
 */
char *
log_readline(FILE * logfile){
    char * line = malloc((MAX_LINE_LENGTH) * sizeof(char));
    if (fgets(line, MAX_LINE_LENGTH, logfile) != NULL)
    {
        return line;
    }
    return (char *)NULL;
}
