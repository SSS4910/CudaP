//C Apache log file parser
//
//Created by:
//              John Stanley
//
//
//
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sss4910.h"



    int parse_line(char* line, Request * outRequest);
    time_t parse_time(char * input);


    static int const HOSTSIZE = 1000;
    static int const CLIENTIDSIZE = 50;
    static int const USERIDSIZE = 150;
    static int const TIMESIZE = 100;
    static int const REQSIZE = 2000;
    static int const CODESIZE = 30;
    static int const SIZESIZE = 30;
    static int const REFERERSIZE = 30;
    static int const USERAGENTSIZE = 30;


int main(int argc, char **argv)
{

    int lineN = 0;
    FILE *fp;
    
    //fp = fopen("bgcsierravlanca_access.log","r");
    fp = fopen("almhuette_access.log", "r");
    //fp = fopen("UofS_access_log","r");
    //fp = fopen("redlug.log","r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    Request out;

    if (fp == NULL) 
    {
        printf("Im sorry invalid file");
        exit(EXIT_FAILURE);
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
       /* out = malloc((sizeof(char)* 
                        (
                        HOSTSIZE+
                        CLIENTIDSIZE+
                        USERIDSIZE+
                        TIMESIZE+
                        REQSIZE+
                        REFERERSIZE+
                        USERAGENTSIZE))
                    + (sizeof(int) * 20));*/

        out.host = (char *)malloc(HOSTSIZE * sizeof(char));    
        out.clientId = (char *)malloc(CLIENTIDSIZE * sizeof(char)); 
        out.userId = (char *)malloc((USERIDSIZE) * sizeof(char));  
        out.time = (char *)malloc(TIMESIZE * sizeof(char));  
        out.req = (char *)malloc((REQSIZE) * sizeof(char));  
        out.referer = (char *)malloc((REFERERSIZE) * sizeof(char));  
        out.userAgent = (char *)malloc(USERAGENTSIZE * sizeof(char));  



        if(parse_line(line,& out) == 0)
        {
            //printf("worked\n");
        }

        // printf("\n %d \n", lineN); 
        
        //printf("H:%s R:%s A:%s T:%s C:%s c:%d S:%d \n \n",out.host, out.clientId, out.userId, out.time, out.req, out.httpReturnCode, out.dataSize );
    
    

        free(out.host);
        free(out.clientId);
        free(out.userId);
        free(out.time);
        free(out.req);
        free(out.referer);
        free(out.userAgent);
        lineN++;
        //free(out);

    }
    //printf("%d    %s \n",lineN,line);
    printf("DONE :D\n");


    fclose(fp);
}

/* 
 *FUNCTION: parse_line
 *---------------------------------
 * Returns a "Request"
 *
 * takes a line from an apache log file and looks at each character in order
 *  it uses an apache log to generate a request struct, which it then passes on to the buffer
 *
 */
int parse_line(char* line, Request * outRequest)
{
        //printf("%s \n",line);
    
    
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
                    outRequest->host[j] = '\0';
                    j =0;
                }
                else
                {
                    outRequest->host[j] = line[i];
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
                    outRequest->clientId[j] = '\0';
                    j =0;
                }
                else
                {
                    outRequest->clientId[j] = line[i];
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
                    outRequest->userId[j] = '\0';
                    j = 0;
                }
                else
                {
                    outRequest->userId[j] = line[i];
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
                    outRequest->time[j] = '\0';
                    j = 0;

                }
                else if(line[i] == ']')
                {
                    flag = 1;
                    outRequest->time[j] = line[i];
                    j++;
                }
                else
                {
                    outRequest->time[j] = line[i];
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
                        outRequest->req[j] = line[i];
                        j++;
                    }
                    else
                    {
                        flag = 0;
                        n = 5;
                        h = 0;
                        outRequest->req[j] = '\0';
                    }
                }
                else if( line[i] == '"')
                {
                    outRequest->req[j] = line[i];
                    j++;
                    flag++;
                }
                else
                {
                    outRequest->req[j] = line[i];
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
                    outRequest->httpReturnCode = atoi(retCode);
                    if(outRequest->httpReturnCode == 0)
                    {
                        //n = 5;
                        for(k = 0; k < h; k++)
                        {
                            outRequest->req[j] = retCode[k];
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
                    outRequest->dataSize = atoi(retSize);
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



/*time_t parse_time(char * input)
{
    /*
      [day/month/year:hour:minute:second zone]
      day = 2*digit
      month = 3*letter
      year = 4*digit
      hour = 2*digit
      minute = 2*digit
      second = 2*digit
      zone = (`+' | `-') 4*digit
    */
/*    time_t time;
     
    int i;
    int n = 0;

    int day = 0;
    int month = 0;
    int year = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int zone = 0;

    for(i = 0;i < len(input); i++)
    {
        switch (n)
        {
            case 0:
                n = 1;
                break;
            case 1:
                if (input(i) == '/')
                {
                    n = 2;
                }
                else
                {
                    day = day + atoi(input(i));
                }
                    

        }

    }

    
}*/
