#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <regex.h>

#include "core.h"
#include "parser.h"
#include "debug.h"
#include "analysis.h"
#include "gopt.h"

// Globals
Buffer buffer1;
Buffer buffer2;
Statistics totalStats;
UniqueRequests uniqueRequests;
int MASTER_SWITCH;

/**
 * FUNCTION: main
 * --------------
 * the entry point of the application
 *
 * @param argc the number of command line arguments
 * @param argv the command line arguments, kept as a pointer to a string
 *
 * @return 0 on successful completion, -1 on failure
 */
int
main(int argc, char** argv){
    int i;
    int err;
    int lineNum = 0;
    char * logLine;
    char * fileName = "../../access.log";
    FILE * logfile;
    FILE * errorFile;

     open_debug_file();

    // Set/Compile regex
    regex_t regex;
    char * regexString;
    regexString = "^[A-Za-z0-9_:.'?#()/&-]* [A-Za-z0-9_-]* [A-Za-z0-9.@_ -]* [\\[][0-3][0-9][/][JFMASOND][aepuco][nbrylgpvtc][/]....[:][0-2][0-9][:][0-5][0-9][:][0-5][0-9] [-+]....[]] [\\\"].*[\\\"] [0-9][0-9]* [0-9-][0-9-]*"; 
    err = regcomp(&regex, regexString, 0);    
    if(err)
    {
        fprintf(stderr, "Regular expression failed to compile\n");
        debug_write("Regular expression failed to compile\n");
        exit(1);
    }

    // parse command line arguments
    if(parse_opt(argc, argv, &fileName)  != TRUE)
    {
        fprintf(stderr, "Non-execution option selected\n");
        exit(0);
    }
    fprintf(stderr, "Analyzing file: %s\n", fileName);

    // Switch MASTER_SWITCH to TRUE
    MASTER_SWITCH = TRUE;

    //Delete output files
    if(delete_output_files() != 0)
    {
        fprintf(stderr, "Error while deleting output files\n");
        debug_write("Error while deleting output files\n");
        exit(1);
    }

    // Initialize totalStats
    totalStats.total200        = 0;
    totalStats.total404        = 0;
    totalStats.totalInjections = 0;
    totalStats.totalVisits     = 0;

    for(i = 0; i < 24; i++)
    {
        totalStats.hourlyAccess[i] = 0;
    }

    for(i = 0; i < 12; i++)
    {
        totalStats.monthlyAccess[i] = 0;
    }

    debug_write("Opening error file\n");
    if((errorFile = fopen("errors.txt", "w")) == NULL)
    {
        debug_write("Unable to create errors.txt\n");
        fprintf(stderr, "Unable to create errors.txt\n");
        return -1;
    }

    debug_write("Opening access log file\n");
    if ((logfile = fopen(fileName, "r")) == NULL)
    {
        debug_write("access log not found, aborting!\n");
        fprintf(errorFile, "access log not found\n");
        printf("%s not found, aborting!\n", fileName);
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

    // initialize uniqueRequests
    uniqueRequests.urls = (URL *)malloc(sizeof(URL)*MAX_UNIQUE_URLS);
    uniqueRequests.currentSize = 0;

    // initialize buffer values
    buffer1.id = 1;
    buffer1.readyRead = FALSE;
    buffer1.readyWrite = TRUE;

    buffer2.id = 2;
    buffer2.readyRead = FALSE;
    buffer2.readyWrite = TRUE;

    // Create analysis thread
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
                logLine = malloc((MAX_LINE_LENGTH) * sizeof(char));
                err = log_readline(logfile, logLine, &regex);
                lineNum++;
                if (feof(logfile))
                {
                    break;
                }
                else if(err == 1)
                {
                    fprintf(stderr, "Error: invalid entry on line %d in log file\n", lineNum);
                    fprintf(errorFile, "Error: invalid entry on line %d : %s\n", lineNum, logLine);
                    --i;
                    continue;
                }
                else if(err == 2)
                {
                    fprintf(stderr, "Error: reader error: %d in log file\n", lineNum);
                    fprintf(errorFile, "Error: reader error %d\n", lineNum);
                    /* close all files for clean exit? */
                    exit(1);
                }
                
                //parse line and add it to the buffer
                err = parse_line(logLine, &buffer1.requests[i]);
                if (err)
                {
                    printf("parse error on line %d\n%s\n", lineNum, logLine);
                    fprintf(errorFile, "Error: parsing line %d : %s\n", lineNum, logLine);
                    --i;
                    continue;
                }
                buffer1.currentSize++;
                free(logLine);

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
        }

        if(buffer2.readyWrite)
        {
            buffer2.currentSize = 0;
            for (i = 0; i < BUFFER_SIZE; i++)
            {
                //insure that the structure is empty
                req_null(&buffer2.requests[i]);

                //read a line
                logLine = malloc((MAX_LINE_LENGTH) * sizeof(char));
                err = log_readline(logfile, logLine, &regex);
                lineNum++;
                if (feof(logfile))
                {
                    break;
                }
                else if(err == 1)
                {
                    fprintf(stderr, "Error: invalid entry on line %d in log file\n", lineNum);
                    fprintf(errorFile, "Error: invalid entry on line %d : %s\n", lineNum, logLine);
                    --i;
                    continue;
                }
                else if(err == 2)
                {
                    fprintf(stderr, "Error: unknown reader error: %d in log file\n", lineNum);
                    fprintf(errorFile, "Error: unknown reader error %d\n", lineNum);
                    /* close all files for clean exit? */
                    exit(1);
                }
                
                //parse line and add it to the buffer
                err = parse_line(logLine, &buffer2.requests[i]);
                if (err)
                {
                    printf("parse error on line %d\n%s\n", lineNum, logLine);
                    fprintf(errorFile, "Error: parsing line %d : %s\n", lineNum, logLine);
                    --i;
                    continue;
                }
                buffer2.currentSize++;
                free(logLine);

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
        }
   
    }

    MASTER_SWITCH = FALSE;
    // wait for any remaining analysis to finish
    pthread_join(analysisThread, NULL);

    #if DEBUG==1
        printf("\nTotal Statistics\n");
        printf("Total 200s: %d\n", totalStats.total200);
        printf("Total 404s: %d\n", totalStats.total404);
        printf("Total injections: %d\n", totalStats.totalInjections);
        printf("Total visits: %d\n", totalStats.totalVisits);
    #endif

    //Write Statistics to file
    if(write_general_stats() != 0)
    {
        fprintf(errorFile, "Error: Unable to write statistics\n");
        exit(1);
    }
    
    fprintf(stderr, "Total number of unique requests: %d\n", uniqueRequests.currentSize);

    //cleanup
    buffer_free(&buffer1);
    buffer_free(&buffer2);
    //free(fileName);
    debug_write("Freeing memory for line buffer\n");
    fclose(logfile);
    //free(uniqueRequests);
    regfree(&regex);
    fclose(errorFile);
    debug_write("Closing access.log file...\n");
    close_debug_file();

    for(i = 0; i < uniqueRequests.currentSize; i++)
    {
        free(uniqueRequests.urls[i].url);
    }
    free(uniqueRequests.urls);

    printf("\nFinished Analysis\n\n");
    return 0;
}


/**
 * FUNCTION: buffer_init
 * -----------------------
 * initialize buffer to store each line of the log
 *
 * @return a pointer to a buffer structure on success, NULL on failure
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

/**
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

/**
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
    request->strTime = (char *) malloc(100 * sizeof(char));
    request->req = (char *) malloc(2000 * sizeof(char));
    request->referer = (char *) malloc(700 * sizeof(char));
    request->userAgent = (char *) malloc(300 * sizeof(char));
    return 0;
}

/**
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
    strcpy(request->strTime, "~");
    strcpy(request->req, "~");
    request->retCode = -1;
    request->dataSize = -1;
    strcpy(request->referer, "~");
    strcpy(request->userAgent, "~");
    return 0;
}


/**
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
    free(request->strTime);
    free(request->req);
    free(request->referer);
    free(request->userAgent);
}

/**
 * FUNCTION: log_readline
 * ----------------------
 * reads next line from provided log file
 *
 * logfile: the log file pointer
 *
 * return:
 *  a newline terminated string containing the next line from file
 */
int
log_readline(FILE * logfile, char * line, regex_t *regex)
{
    int status = 0;
    int err;
    if (fgets(line, MAX_LINE_LENGTH, logfile) != NULL)
    {
        /* REGULAR EXPRESSION */
        err = regexec(regex, line, 0, NULL, 0);
        if(err == REG_NOMATCH)
        {
            debug_write("Failed regex match\n");
            status = 1;
        }
        return status;
    }

    status = 2;
    return status;
}

int delete_output_files()
{
    //404Data.txt
    if(access("404Data.txt", F_OK) == 0)
    {
        int err = remove("404Data.txt");
        if(err != 0)
        {
            debug_write("Failed to delete 404Data.txt\n");
            return 1;
        }
    }

    //stats.txt
    if(access("stats.txt", F_OK) == 0)
    {
        int err = remove("stats.txt");
        if(err != 0)
        {
            debug_write("Failed to delete stats.txt\n");
            return 1;
        }
    }

    //errorFile.txt
    if(access("errorFile.txt", F_OK) == 0)
    {
        int err = remove("errorFile.txt");
        if(err != 0)
        {
            debug_write("Failed to delete errorFile.txt\n");
            return 1;
        }
    }

    

    return 0;
}

/**
    FUNCTION: write_general_stats
    -----------------------------
    Writes the global Statistics struct, totalStats,
    to an output file, stats.txt.
*/
int write_general_stats()
{

    FILE *statsFile; 
    if((statsFile = fopen("stats.txt", "w")) == NULL)
    {
        debug_write("Unable to create stats.txt file\n");
        //fprintf(errorFile, "Unable to create output file\n");
        return 1;
    }

    fprintf(statsFile, "%d;%d;%d;%d\n", totalStats.total200,
                                        totalStats.total404,
                                        totalStats.totalInjections,
                                        totalStats.totalVisits);
    // Write hours to file
    int i;
    for(i = 0; i < 24;i++)
    {
        fprintf(statsFile, "%lld;", totalStats.hourlyAccess[i]);
    }
    fprintf(statsFile, "\n");

    // Write Months to file
    for(i = 0; i < 12;i++)
    {
        fprintf(statsFile, "%lld;", totalStats.monthlyAccess[i]);
    }
    fprintf(statsFile, "\n");

    #if DEBUG==1
        // Testing time stats
        for(i = 0; i < 24;i++)
        {
            fprintf(stderr, "Hour: %d : %lld\n", i, totalStats.hourlyAccess[i]);
        }
        fprintf(stderr, "\n");

        for(i = 0; i < 12;i++)
        {
            fprintf(stderr, "Month: %d : %lld\n", i, totalStats.monthlyAccess[i]);
        }
        fprintf(stderr, "\n");
    #endif

    fclose(statsFile);

    return 0;
}