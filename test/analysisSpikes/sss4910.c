#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>

#include "core.h"
#include "parser.h"
#include "debug.h"
#include "analysis.h"

// Globals
Buffer buffer1;
Buffer buffer2;
Statistics totalStats;
//Queue404 queue404;
int MASTER_SWITCH;

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

    MASTER_SWITCH = TRUE;

    // Initialize Queue404
    /*queue404.requests = (Request *)malloc(sizeof(Request) * BUFFER_SIZE);// would prefer this be more dynamic
    queue404.currentSize  = 0;
    queue404.currentIndex = 0;*/

    // Initialize totalStats
    totalStats.total200        = 0;
    totalStats.total404        = 0;
    totalStats.totalInjections = 0;
    totalStats.totalVisits     = 0;

    //getopt()
    open_debug_file();

    debug_write("Opening access.log file\n");
    if ((logfile = fopen("../../../UofS_access_log", "r")) == NULL)
    {
        debug_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }

    // Initialize buffers
    debug_write("Allocating memory for buffer1\n");
    err = buffer_init(&buffer1);
    if (err)
    {
        debug_write("buffer1 allocation failure, aborting!\n");
        return -1;
    }

    debug_write("Allocating memory for buffer2\n");
    err = buffer_init(&buffer2);
    if (err)
    {
        debug_write("buffer2 allocation failure, aborting!\n");
        return -1;
    }
    buffer1.id = 1;
    buffer1.readyRead = FALSE;
    buffer1.readyWrite = TRUE;

    buffer2.id = 2;
    buffer2.readyRead = FALSE;
    buffer2.readyWrite = TRUE;

    /*

    strcpy(buffer.requests[0].host, "127.0.0.1");

    printf("%s\n", buffer.requests[0].host);

    req_null(&buffer.requests[0]);

    printf("%s\n", buffer.requests[0].host);

    */

    /* SPAWN ANALYSIS THREAD HERE */
    pthread_t analysisThread;
    pthread_create(&analysisThread, NULL, manage_data, NULL);


    while (!feof(logfile))
    {
        if(buffer1.readyWrite)
        {
            buffer1.currentSize = 0;
            for (i = 0; i < BUFFER_SIZE; i++)
            {
                //insure that the structure is empty
                req_null(&buffer1.requests[i]);

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
                err = parse_line(logline, &buffer1.requests[i]);
                if (err)
                {
                    printf("parse error on line %d\n", lineNum);
                }
                buffer1.currentSize++;
                free(logline);

                /*#if DEBUG==1
                    printf("%s\n", buffer1.requests[i].host);
                    printf("%s\n", buffer1.requests[i].clientId);
                    printf("%s\n", buffer1.requests[i].userId);
                    printf("%s\n", buffer1.requests[i].time);
                    printf("%s\n", buffer1.requests[i].req);
                    printf("%d\n", buffer1.requests[i].retCode);
                    printf("%d\n", buffer1.requests[i].dataSize);
                    printf("%s\n", buffer1.requests[i].referer);
                    printf("%s\n", buffer1.requests[i].userAgent);
                #endif*/

            }

            buffer1.readyWrite = FALSE;
            buffer1.readyRead = TRUE;

            //manage_data(); // this should actually be a thread
        }

        if(buffer2.readyWrite)
        {
            buffer2.currentSize = 0;
            for (i = 0; i < BUFFER_SIZE; i++)
            {
                //insure that the structure is empty
                req_null(&buffer2.requests[i]);

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
                err = parse_line(logline, &buffer2.requests[i]);
                if (err)
                {
                    printf("parse error on line %d\n", lineNum);
                }
                buffer2.currentSize++;
                free(logline);

                /*#if DEBUG==1
                    printf("%s\n", buffer2.requests[i].host);
                    printf("%s\n", buffer2.requests[i].clientId);
                    printf("%s\n", buffer2.requests[i].userId);
                    printf("%s\n", buffer2.requests[i].time);
                    printf("%s\n", buffer2.requests[i].req);
                    printf("%d\n", buffer2.requests[i].retCode);
                    printf("%d\n", buffer2.requests[i].dataSize);
                    printf("%s\n", buffer2.requests[i].referer);
                    printf("%s\n", buffer2.requests[i].userAgent);
                #endif*/

            }

            buffer2.readyWrite = FALSE;
            buffer2.readyRead = TRUE;

            //manage_data(); // this should actually be a thread
        }
   
    }

    MASTER_SWITCH = FALSE;
    /* JOIN ANALYSIS THREAD HERE */
    pthread_join(analysisThread, NULL);

    #if DEBUG==1
        printf("\nTotal Statistics\n");
        printf("Total 200s: %d\n", totalStats.total200);
        printf("Total 404s: %d\n", totalStats.total404);
        printf("Total injections: %d\n", totalStats.totalInjections);
        printf("Total visits: %d\n", totalStats.totalVisits);

        /*printf("\nList of 404 requests\n");
        printf("Total 404 requests: %d\n", queue404.currentSize);
        for(i = 0; i < queue404.currentSize; i++)
        {
            printf("%d: %s %s %s %d\n", i+1, queue404.requests[i].host, queue404.requests[i].req, queue404.requests[i].time, queue404.requests[i].retCode);
        }*/
    #endif

    //cleanup
    buffer_free(&buffer1);
    buffer_free(&buffer2);

    debug_write("Freeing memory for line buffer\n");
    fclose(logfile);

    debug_write("Closing access.log file...\n");
    close_debug_file();

    printf("\nEND OF PROGRAM\n");

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
    request->referer = (char *) malloc(700 * sizeof(char));
    request->userAgent = (char *) malloc(300 * sizeof(char));
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
    strcpy(request->req, "~");
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
        fprintf(stderr, "%s\t*********************************\n", line);
        return line;
    }
    return (char *)NULL;
}
