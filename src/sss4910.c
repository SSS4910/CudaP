#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

Buffer *
buffer_init();

void
free_req();

char *
log_readline(FILE *);

/*
 * FUNCTION: main
 * --------------
 * the entry point of the application
 *
 * argc: the number of command line arguments
 * argv: the command line arguments, kept as a pointer to a string
 *
 * returns: 0 on successful completion, -1 on failure
 */
int
main(int argc, char** argv){
    int i;
    int err;
    int lineNum = 0;
    char * logline;
    FILE * logfile;
    Buffer * buffer;
    Request LogLine;

    //getopt()
    open_debug_file();

    if ((logfile = fopen("access.log", "r")) == NULL)
    {
        debug_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }
    debug_write("Opening access.log file\n");

    buffer = buffer_init();
    buffer->available = TRUE;
    debug_write("Allocating memory for buffer\n");

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
                break;
            }
            lineNum++;
            //parse line and add it to the buffer
            err = parse_line(logline, &LogLine);
            if (err)
            {
                printf("parse error on line %d\n", lineNum);
            }
            buffer->requests[i] = LogLine;
            buffer->currentSize++;
        }
    }

    //cleanup
    debug_write("Closing access.log file...\n");
    fclose(logfile);

    for (i = 0; i < BUFFER_SIZE; i++)
    {
        free_req(&buffer->requests[i]);
    }
    free(buffer->requests);
    free(buffer);
    debug_write("Freeing memory for line buffer\n");
 
    close_debug_file();
    return 0;
}


/*
 * FUNCTION: buffer_init
 * -----------------------
 * initialize buffer to store each line of the log
 *
 * return: a pointer to a buffer structure on success, NULL on failure
 */
Buffer * 
buffer_init(){
    Buffer * buffer;
 
    buffer = malloc(sizeof(Buffer));
    if (buffer == NULL)
    {
        return (Buffer * ) NULL;
    }
    buffer->requests = malloc((BUFFER_SIZE) * sizeof(Request));
    return buffer;
}


/*
 * FUNCTION: free_req
 * ------------------
 * frees request structure data
 *
 * request: pointer to a Request structure to be freed
 */
void
free_req(Request *request){
    free(request->host);
    free(request->clientId);
    free(request->userId);
    //time_t time;
    free(request->req);
    //int retCode;
    //int dataSize;
    free(request->referer);
    free(request->userAgent);
    free(request);
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
