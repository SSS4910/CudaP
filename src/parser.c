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
parse_line(char * line, Request *outRequest){
    //if(debug2) printf("%s \n",line);
    
    
        //counters
    int currentField = 0;
    int lengthOfCurrentField = 0;
    int i = 0;
    int lengthOfReq = 0;
    int k = 0;
    
    int flag = 0;
    

    char* retCode;
    retCode = (char *)malloc((30) * sizeof(char));  
       
    char* retSize;
    retSize = (char *)malloc((30) * sizeof(char));


    outRequest->referer[0] = '~';
    outRequest->referer[1] = '\0';

    outRequest->userAgent[0] = '~';
    outRequest->userAgent[1] = '\0';

         // loop for every character in the input line 
    for(i =0; i < strlen(line); i++)
    {

            //this parses and breaks down the line into fields, each case is a different 
        switch(currentField)
        {

                //field one 
                //      (%h) host field
                //          this is either and IP or a website
                //
                //          terminated by ' '
            case 0:
                    //if(debug3)printf(" In field one \n");
                
                if (line[i] == ' ')
                {
                    currentField = 1;
                    outRequest->host[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;
                }
                else
                {
                    outRequest->host[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
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
                    if(line[i+1] == '[')
                    {
                    currentField = 2;
                    outRequest->clientId[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;
                    }
                    else
                    {
                        outRequest->clientId[lengthOfCurrentField] = line[i];
                        lengthOfCurrentField++;
                    }
                }
                else
                {
                    outRequest->clientId[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                break;

                //field three
                //      (%u)    user id
                //      
                //      terminated by a ' '
            case 2:
                //if(debug3) printf(" In field three \n");

                if(line[i] == ' ')
                {
                    currentField = 3;
                    outRequest->userId[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;
                }
                else
                {
                    outRequest->userId[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
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
                    //if(debug3)printf(" In field four \n");
                
                if(lengthOfCurrentField >= 30)
                {
                    //time cannot be bigger than 23
                    return -1;
                }
                if(flag == 1)
                {
                    currentField = 4;
                    flag = 0;
                    outRequest->time[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;

                }
                else if(line[i] == ']')
                {
                    flag = 1;
                    outRequest->time[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                else
                {
                    outRequest->time[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
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
                    //if(debug3) printf(" In field five \n");

                if(flag == 2)
                {
                    if (line[i] != ' ')
                    {
                        flag = 1;
                        currentField = 4;
                        outRequest->req[lengthOfCurrentField] = line[i];
                        lengthOfCurrentField++;
                    }
                    else
                    {
                        flag = 0;
                        outRequest->req[lengthOfCurrentField] = '\0';
                        lengthOfReq = lengthOfCurrentField;
                        currentField = 5;
                        lengthOfCurrentField = 0;
                    }
                }
                else if( line[i] == '"')
                {

                    outRequest->req[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                    flag++;
                }
                else
                {
                    outRequest->req[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
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
                    //if(debug3)printf(" In field six \n");

                if(line[i] == ' ')
                {
                    retCode[lengthOfCurrentField] = '\0';
                    outRequest->retCode = atoi(retCode);
                    if(outRequest->retCode == 0)
                    {

                        outRequest->req[lengthOfReq] = ' ';
                        lengthOfReq++;
                        for(k = 0; k < lengthOfCurrentField; k++)
                        {
                            outRequest->req[lengthOfReq] = retCode[k];
                            lengthOfReq++;
                        }
                        lengthOfCurrentField = 0;
                    }
                    else
                    {
                        currentField = 6;
                        lengthOfCurrentField = 0;
                    }
                }
                else
                {
                    
                    retCode[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
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
                    //if(debug3) printf(" In field seven \n");

                if(line[i] == ' ' || line[i] == '\n')
                {
                    currentField = 7;
                    retSize[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;
                    outRequest->dataSize = atoi(retSize);
                }
                else
                {
                    retSize[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                break;

           //EXTENDED LOG FORMAT FOLLOWS 
            case 7:
                    //if(debug3) printf(" In field eight \n");
                
                if(flag == 2)
                {
                    flag = 0;
                    outRequest->referer[lengthOfCurrentField] = '\0';
                    currentField = 8;
                    lengthOfCurrentField = 0;
                }
                else if( line[i] == '"')
                {

                    //outRequest->referer[lengthOfCurrentField] = line[i];
                    //lengthOfCurrentField++;
                    flag++;
                }
                else
                {
                    outRequest->referer[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                break;
             case 8:
                    //if(debug3) printf(" In field eight \n");
                
                if(flag == 2)
                {
                    flag = 0;
                    outRequest->userAgent[lengthOfCurrentField] = '\0';
                    currentField = 9;
                    lengthOfCurrentField = 0;
                }
                else if( line[i] == '"')
                {

                    //outRequest->userAgent[lengthOfCurrentField] = line[i];
                    //lengthOfCurrentField++;
                    flag++;
                }
                else
                {
                    outRequest->userAgent[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                break;
            

        }   


    }

    //if(debug1) printf("   Inside: H:%s R:%s A:%s T:%s C:%s c:%d S:%d r:%s U:%s \n \n",outRequest->host, outRequest->clientId, outRequest->userId, outRequest->time, outRequest->req, outRequest->retCode, outRequest->dataSize, outRequest->referer, outRequest->userAgent );
    
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



/* FUCTION: stripreq
*---------------------
*    returns a req string that has its first and last part stripped off
*    for easier use in analysis
*
*/
char * stripreq(char * inputString )
{
    int spaces = 0;
    int i;
    int currentSpace = 0;
    char * buffer;
    buffer = (char *)malloc((2000) * sizeof(char));  

   
    int currentLengthOfBuffer = 0;
    
    //if(stripdebug2) printf("A"); 
    
    for(i = 0; i < strlen(inputString);i++)
    {
        if (inputString[i] == ' ')
        {
            spaces++;
        }
    }
    

    //if(stripdebug2) printf("B %d ",spaces);
    
    for(i = 0; i < strlen(inputString); i++)
    {
        //if(stripdebug2) printf("C%d ", i);
            
        if(currentSpace == 0)
        {
            if(inputString[i] == ' ')
            {
                currentSpace++;
            }
        }
        else if(currentSpace == spaces)
        {
            if(spaces == 1)
            {
                buffer[currentLengthOfBuffer] = inputString[i];
                currentLengthOfBuffer++;
            }
            else
            {
                buffer[currentLengthOfBuffer] = '\0';
                break;
            }
        }
        else
        {
            if(inputString[i] == ' ')
            {
                currentSpace++;
            }
            buffer[currentLengthOfBuffer] = inputString[i];
            currentLengthOfBuffer++;
        }   
    }
    if(spaces == 1)
    {
        buffer[currentLengthOfBuffer-1] = '\0';
    }

    //if(stripdebug) printf("\n\n\n stripout:\n %s \n\n%s \n",inputString,buffer);
    
    return buffer;
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
