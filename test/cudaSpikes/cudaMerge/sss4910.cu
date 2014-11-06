#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "core.h"
#include "parser.h"
#include "debug.h"
#include "cuda.cuh"

//this shouldn't be here, but...
int
parse_line(char *, Request *);



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
    //open_debug_file();

    fprintf(stdout, "Nothing has happened yet\n");

    // choose cuda device
    cuda_setup(COMPUTE_ABILITY);

    //debug_write("Opening access.log file\n");
    if ((logfile = fopen("../../../../access.log", "r")) == NULL)
    {
        //debug_write("access.log not found, aborting!\n");
        printf("access.log not found, aborting!\n");
        return -1;
    }

    fprintf(stdout, "Server log was opened\n");

    //debug_write("Allocating memory for buffer\n");
    err = buffer_init(&buffer);
    if (err)
    {
        //debug_write("buffer allocation failure, aborting!\n");
        return -1;
    }
    buffer.available = TRUE;
    fprintf(stdout, "Buffer created\n");
    /*

    strcpy(buffer.requests[0].host, "127.0.0.1");

    printf("%s\n", buffer.requests[0].host);

    req_null(&buffer.requests[0]);

    printf("%s\n", buffer.requests[0].host);

    */
    fprintf(stdout, "Doing the loop\n");
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
                //printf("parse error on line %d\n", lineNum);
            }
            buffer.currentSize++;
            free(logline);
            /*printf("%s\n", buffer.requests[i].host);
            printf("%s\n", buffer.requests[i].clientId);
            printf("%s\n", buffer.requests[i].userId);
            printf("%s\n", buffer.requests[i].time);
            printf("%s\n", buffer.requests[i].req);
            printf("%d\n", buffer.requests[i].retCode);
            printf("%d\n", buffer.requests[i].dataSize);
            fflush(stdout);*/
            //fprintf(stdout, "Test in main... %d\n", (&buffer.requests[i].host) -1 );

        }

        // call to cuda analysis
        //fprintf(stderr, "Size %d\n", (int)sizeof(buffer));
        int cudaSucces = analyze_data(&buffer);
        if(!cudaSucces)
        {
            printf("Cuda Module Failed to Properly Execute\n");
        }
        fprintf(stdout, "Forced end\n");
        exit(0);
    }

    fprintf(stdout, "Loop ended\n");

    //cleanup
    buffer_free(&buffer);
    //debug_write("Freeing memory for line buffer\n");

    fclose(logfile);
    //debug_write("Closing access.log file...\n");
    //close_debug_file();
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
    buffer->requests = (Request *)malloc((BUFFER_SIZE) * sizeof(Request));
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (req_init(&buffer->requests[i]))
        {
            //debug_write("request struct allocation error!\n");
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
    //debug_write("free requests pointer\n");
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
    //debug_write("allocating request structure field memory!\n");
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
    //debug_write("freeing request structure field memory\n");
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
    char * line = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    if (fgets(line, MAX_LINE_LENGTH, logfile) != NULL)
    {
        return line;
    }
    return (char *)NULL;
}






// should not be here
int
parse_line(char * line, Request *request){
    //printf("%s \n",line);

    /*static int const HOSTSIZE = 1000;
    static int const CLIENTIDSIZE = 50;
    static int const USERIDSIZE = 150;
    static int const TIMESIZE = 100;
    static int const REQSIZE = 2000;
    static int const CODESIZE = 30;
    static int const SIZESIZE = 30;
    static int const REFERERSIZE = 30;
    static int const USERAGENTSIZE = 30;*/
    
        //counters
    int n = 0;
    int j = 0;
    int i = 0;
    int h = 0;
    int k = 0;

    

        // alocate memory and create the Request that will be returned
    
    //Request * outRequest;
    //outRequest = malloc(sizeof(Request*));
    /*outRequest = malloc((sizeof(char)* 
                        (
                        HOSTSIZE+
                        CLIENTIDSIZE+
                        USERIDSIZE+
                        TIMESIZE+
                        REQSIZE+
                        REFERERSIZE+
                        USERAGENTSIZE))
                    + (sizeof(int) * 20));
    
    outRequest->host = (char *)malloc(HOSTSIZE * sizeof(char));    
      
    outRequest->clientId = (char *)malloc(CLIENTIDSIZE * sizeof(char)); 
     
    outRequest->userId = (char *)malloc((USERIDSIZE) * sizeof(char));  
    
    outRequest->time = (char *)malloc(TIMESIZE * sizeof(char));  
    
    outRequest->req = (char *)malloc((REQSIZE) * sizeof(char));  
    */
    char* retCode;
    retCode = (char *)malloc((30) * sizeof(char));  
       
    char* retSize;
    retSize = (char *)malloc((30) * sizeof(char));  
    /*   
    outRequest->referer = (char *)malloc((REFERERSIZE) * sizeof(char));  

    outRequest->userAgent = (char *)malloc(USERAGENTSIZE * sizeof(char));   
    */
    int flag = 0;
    


        // loop for every character in the input line 
    for(i =0; i < strlen(line); i++)
    {

            //fflush(stdout);

            //this parses and breaks down the line into fields, each case is a different 
        switch(n)
        {

                //field one 
                //      (%h) host field
                //          this is either and IP or a website
                //
                //          terminated by ' '
            case 0:
                    //printf(" In field one \n");
                
                if (line[i] == ' ')
                {
                    n = 1;
                    request->host[j] = '\0';
                    j =0;
                }
                else
                {
                    request->host[j] = line[i];
                    j++;
                }
                break;

                //field two 
                //      (%l) RFC
                //          request 1413
                //
                //          terminated by ' '
            case 1:
                    //printf(" In field two \n");

                if(line[i] == ' ')
                {
                    n = 2;
                    request->clientId[j] = '\0';
                    j =0;
                }
                else
                {
                    request->clientId[j] = line[i];
                    j++;
                }
                break;

                //field three
                //      (%u)    user id
                //      
                //      terminated by a ' '
            case 2:
                //printf(" In field three \n");

                if(line[i] == ' ')
                {
                    n = 3;
                    request->userId[j] = '\0';
                    j = 0;
                }
                else
                {
                    request->userId[j] = line[i];
                    j++;
                }
                break;

                //field four 
                //      (%t) time
                //      this is the time field 
                //      
                //      [day/month/year:hour:minute:second zone]
                //      day = 2*digit
                //      month = 3*letter
                //      year = 4*digit
                //      hour = 2*digit
                //      minute = 2*digit
                //      second = 2*digit
                //      zone = (`+' | `-') 4*digit
                //
                //      terminated by a ']'
            case 3:
                    //printf(" In field four \n");
                if(flag == 1)
                {
                    n = 4;
                    flag = 0;
                    request->time[j] = '\0';
                    j = 0;

                }
                else if(line[i] == ']')
                {
                    flag = 1;
                    request->time[j] = line[i];
                    j++;
                }
                else
                {
                    request->time[j] = line[i];
                    j++;
                }
                break;
           
                //field five
                //      (\"%r\") request line
                //
                //      altho the program begins and ends with a '"' you cannot just assume that
                //      you have hit the end of the feild becuase you hit a second '"' this is why
                //      field six has to check and make sure field five(request line) has ended
                //
                //      terminated by 2nd '"' 
                //      
                //      PROBLAMATIC

            case 4:
                    //printf(" In field five \n");

                if(flag == 2)
                {
                    if (line[i] != ' ')
                    {
                        flag = 1;
                        n = 4;
                        request->req[j] = line[i];
                        j++;
                    }
                    else
                    {
                        flag = 0;
                        n = 5;
                        h = 0;
                        request->req[j] = '\0';
                    }
                }
                else if( line[i] == '"')
                {
                    request->req[j] = line[i];
                    j++;
                    flag++;
                }
                else
                {
                    request->req[j] = line[i];
                    j++;
                }
                break;

                //field six 
                //      (%>s) status code
                //
                //      this is a relatively simple field that usually returns 200 or 404
                //          this also checks to make sure the arnt still looking at the request 
                //          line because of a false positve
                //
                //          terminated by a ' '
                //
            case 5:
                    //printf(" In field six \n");

                if(line[i] == ' ')
                {
                    retCode[h] = '\0';
                    request->retCode = atoi(retCode);
                    if(request->retCode == 0)
                    {
                        //n = 5;
                        for(k = 0; k < h; k++)
                        {
                            request->req[j] = retCode[k];
                            j++;
                        }
                        h = 0;
                    }
                    else
                    {
                        n = 6;
                        h = 0;
                        j = 0;
                    }
                }
                else
                {
                    
                    retCode[h] = line[i];
                    h++;
                }
                break;

                //  feild seven
                //      (%b) size of data
                //
                //      this is just how much data was returned 
                //
                //      terminated by a ' ' or a '\n' for "short" logs
                //
            case 6:
                    //printf(" In field seven \n");

                if(line[i] == ' ' || line[i] == '\n')
                {
                    n = 7;
                    retSize[h] = '\0';
                    h = 0;
                    request->dataSize = atoi(retSize);
                }
                else
                {
                    retSize[h] = line[i];
                    h++;
                }
                break;

                //COMBINED FORMAT LOG DATA STILL NEEDED

        }   


    }

    //printf("   Inside: H:%s R:%s A:%s T:%s C:%s c:%d S:%d \n \n",outRequest->host, outRequest->clientId, outRequest->userId, outRequest->time, outRequest->req, outRequest->httpReturnCode, outRequest->dataSize );
    
    
    /*free(outRequest.host);
    free(outRequest.rfc);
    free(outRequest.auth);
    free(outRequest.time);
    free(outRequest.req);*/
    free(retCode);
    free(retSize);
    /*free(outRequest.referer);
    free(outRequest.userAgent);*/

    return 0;
}
