#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "parser.h"
#include "debug.h"

/*
 * FUNCTION: parse_line
 * --------------------
 * parses a NCSA Common Log Format string into a Request structure
 *
 * line: a string containing a NCSA Common Log Format access line
 * request: a pointer to a structure to hold parsed data
 *
 * returns:
 *  0 on success
 */
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

/*
 * FUNCTION: slice
 * ---------------
 * returns a slice of given string from start offset to end offset
 *
 * line: a char pointer to be sliced
 * so: the start offset (inclusive)
 * eo: the end offset (inclusive)
 */
char *
slice(char *line, int so, int eo){
    if (eo > strlen(line))
    {
        printf("out of bounds error\n");
        return (char *) NULL;
    }
    int start = so;
    int size = (eo - so) + 1;
    char *slice = malloc(size * sizeof(char));
    int i;
    for (i = 0; i < size; i++)
    {
        slice[i] = line[start++];
    }
    slice[size] = '\0';
    return slice;
}
