#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

int
buffer_init(Buffer *);

void
buffer_free(Buffer *);

int
req_init(Request *);

int
req_null(Request *);

void
req_free(Request *);

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
    Buffer buffer;

    //getopt()
    open_debug_file();

    debug_write("Opening access.log file\n");
    if ((logfile = fopen("access.log", "r")) == NULL)
    {
        debug_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }

    debug_write("Allocating memory for buffer\n");
    err = buffer_init(&buffer);
    if (err)
    {
        debug_write("buffer allocation failure, aborting!\n");
        return -1;
    }
    buffer.available = TRUE;

    strcpy(buffer.requests[0].host, "127.0.0.1");

    printf("%s\n", buffer.requests[0].host);

    req_null(&buffer.requests[0]);

    printf("%s\n", buffer.requests[0].host);

    /*

    while (!feof(logfile))
    {
        buffer.currentSize = 0;
        for (i = 0; i < BUFFER_SIZE; i++)
        {
            //insure that the structure is empty
            req_null(&buffer.requests[i]);
            //read a line
            logline = log_readline(logfile);
            if ((logline == (char *) NULL) && feof(logfile))
            {
                //printf("hit eof\n");
                break;
            }
            lineNum++;
            //printf("%s\n", logline);
            //parse line and add it to the buffer
            err = parse_line(logline, &buffer.requests[i]);
            if (err)
            {
                printf("parse error on line %d\n", lineNum);
            }
            buffer.currentSize++;
            //printf("%s\n", buffer->requests[i].host);
            //printf("%s\n", buffer->requests[i].clientId);
            //printf("%s\n", buffer->requests[i].userId);
            //printf("%s\n", buffer->requests[i].time);
            //printf("%s\n", buffer->requests[i].req);
            //printf("%d\n", buffer->requests[i].retCode);
            //printf("%d\n", buffer->requests[i].dataSize);
        }
    }
    */

    //cleanup
    buffer_free(&buffer);
    debug_write("Freeing memory for line buffer\n");

    fclose(logfile);
    debug_write("Closing access.log file...\n");
    close_debug_file();
    return 0;
}


/*
 * FUNCTION: buffer_init
 * -----------------------
 * initialize buffer to store each line of the log
 *
 * return:
 *  a pointer to a buffer structure on success, NULL on failure
 */
int 
buffer_init(Buffer * buffer){
    int i;
    buffer->requests = malloc((BUFFER_SIZE) * sizeof(Request));
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (req_init(&buffer->requests[i]))
        {
            debug_write("request struct allocation error!\n");
            return 1;
        }
    }
    return 0;
}

/*
 * FUNCTION: buffer_free
 * ---------------------
 * frees memory allocated to buffer
 */
void
buffer_free(Buffer * buffer){
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        req_free(&buffer->requests[i]);
    }
    debug_write("free requests pointer\n");
    free(buffer->requests);
}

/*
 * FUNCTION: req_init
 * ------------------
 * allocates memory for a request
 *
 * request: Request structure requiring memory
 *
 * return:
 *  0 on success
 */
int
req_init(Request *request)
{
    debug_write("allocating request structure field memory!\n");
    request->host = (char *) malloc(1000 * sizeof(char));
    request->clientId = (char *) malloc(50 * sizeof(char));
    request->userId = (char *) malloc(150 * sizeof(char));
    request->time = (char *) malloc(100 * sizeof(char));
    request->req = (char *) malloc(2000 * sizeof(char));
    request->referer = (char *) malloc(30 * sizeof(char));
    request->userAgent = (char *) malloc(30 * sizeof(char));
    return 0;
}

/*
 * FUNCTION: req_null
 * ------------------
 * tombstones a request structure
 *
 * return:
 *  0 on success
 */
int
req_null(Request *request)
{
    strcpy(request->host, "~");
    strcpy(request->clientId, "~");
    strcpy(request->userId, "~");
    strcpy(request->time, "~");
    strcpy(request->req = "~");
    request->retCode = -1;
    request->dataSize = -1;
    strcpy(request->referer, "~");
    strcpy(request->userAgent, "~");
    return 0;
}


/*
 * FUNCTION: req_free
 * ------------------
 * frees individual fields of provided request
 *
 * request: Request structure to free
 */
void
req_free(Request *request)
{
    debug_write("freeing request structure field memory\n");
    free(request->host);
    free(request->clientId);
    free(request->userId);
    free(request->time);
    free(request->req);
    free(request->referer);
    free(request->userAgent);
}

/*
 * FUNCTION: log_readline
 * ----------------------
 * reads next line from provided log file
 *
 * logfile: the log file pointer
 *
 * return:
 *  a newline terminated string containing the next line from file
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
