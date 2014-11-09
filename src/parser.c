#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "parser.h"
#include "debug.h"

time_t parse_time(char * input);




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
int parse_line(char * line, Request *outRequest)
{
        //if(debug2) printf("%s \n",line);
    
    if(strlen(line)< 30)
    {
        printf("ERROR: parse line - Bad input\n");
        return 1;
    }

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
                    //if(debug3)printf(" In field two \n");

                if(line[i] == ' ')
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
                break;

                //field three
                //      (%u)    user id
                //      
                //      terminated by a ' '
            case 2:
                	//if(debug3) printf(" In field three \n");
                
                if(line[i] == ' ')
                {
                    if(line[i+1] == '[')
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
                    return 1;
                }
                if(flag == 1)
                {
                    currentField = 4;
                    flag = 0;
                    outRequest->strTime[lengthOfCurrentField] = '\0';
                    lengthOfCurrentField = 0;

                }
                else if(line[i] == ']')
                {
                    flag = 1;
                    outRequest->strTime[lengthOfCurrentField] = line[i];
                    lengthOfCurrentField++;
                }
                else
                {
                    outRequest->strTime[lengthOfCurrentField] = line[i];
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

    //if(debug1) printf("   Inside: H:%s R:%s A:%s T:%s C:%s c:%d S:%d r:%s U:%s \n \n",outRequest->host, outRequest->clientId, outRequest->userId, outRequest->time, outRequest->req, outRequest->httpReturnCode, outRequest->dataSize, outRequest->referer, outRequest->userAgent );
    
    /*free(outRequest.host);
    free(outRequest.rfc);
    free(outRequest.auth);
    free(outRequest.time);
    free(outRequest.req);*/
    free(retCode);
    free(retSize);
    /*free(outRequest.referer);
    free(outRequest.userAgent);*/

    
    if(outRequest->strTime[0] == '~')
    {
        printf("%s\n",line);
        return 1;
    }
    
    outRequest->time = parse_time(outRequest->strTime);
    
    //printf("time_t: %s",asctime(localtime(&outRequest->time))); 

 
            
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
 * FUNCTION: Parse Time
 * ------------------------------------------------------------
 * input: a string that is the apache format time
 *
 * return: a time_t representation of the time
 *
 */
time_t parse_time(char * input)
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

    if(input[0] == '~')
    {
        return (time_t) 0;
    }

    struct tm tempest;
    time_t outtime;

    int i;
    int j = 0;
    int n = 0;

    /*int day = -2;
    int month = -2;
    int year = -2;
    int hour = -2;
    int minute = -2;
    int second = -2;
    */


    //int zone = -22;
    
    
    char* temp;
    temp = (char*) malloc(5 * sizeof(char));   


        //first step is to break down each part of the time field
    for(i = 0;i < strlen(input); i++)
    {
        switch (n)
        {
            case 0:
                if(input[i] != '[')
                {
                    printf("ERROR %s \n",input);
                }
                n = 1;
                j = 0;
                break;

               //DAY!!! 
            case 1:
                if (input[i] == '/')
                {
                    temp[j+1] = '\0';
                    tempest.tm_mday = atoi(temp);
                    n = 2;
                    j = 0;
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;


                //MONTH 
            case 2:
                if(input[i] == '/')
                {
                    temp[j] = '\0';
                    if(strlen(temp) != 3)
                    {
                        printf("ERROR MONTH\n");
                    }
                    else
                    {
                        n = 3;
                        j = 0;

                            //printf("%s \n",temp);
                        if(strcmp(temp,"Jan") == 0)
                        {
                            tempest.tm_mon = 0;
                        }
                        else if(strcmp(temp,"Feb") == 0)
                        {
                            tempest.tm_mon = 1;
                        }
                        else if(strcmp(temp,"Mar") == 0)
                        {
                            tempest.tm_mon = 2;
                        }
                        else if(strcmp(temp,"Apr") == 0)
                        {
                            tempest.tm_mon = 3;
                        }
                        else if(strcmp(temp,"May") == 0)
                        {
                            tempest.tm_mon = 4;
                        }
                        else if(strcmp(temp,"Jun") == 0)
                        {
                            tempest.tm_mon = 5;
                        }
                        else if(strcmp(temp,"Jul") == 0)
                        {
                            tempest.tm_mon = 6;
                        }
                        else if(strcmp(temp,"Aug") == 0)
                        {
                            tempest.tm_mon = 7;
                        }
                        else if(strcmp(temp,"Sep") == 0)
                        {
                            tempest.tm_mon = 8;
                        }
                        else if(strcmp(temp,"Oct") == 0)
                        {
                            tempest.tm_mon = 9; 
                        }
                        else if(strcmp(temp,"Nov") == 0)
                        {
                            tempest.tm_mon = 10;
                        }
                        else if(strcmp(temp,"Dec") == 0)
                        {
                            tempest.tm_mon = 11;
                        }
                        else 
                        {
                            printf("ERROR: parse_time\n");
                            //return 0;
                            tempest.tm_mon = -1;
                        }

                    }
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;
            
                
                //YEAR
            case 3:
                if (input[i] == ':')
                {
                    temp[j] = '\0';
                    tempest.tm_year = (atoi(temp) - 1900);
                    n = 4;
                    j = 0;
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;
                
                //HOUR
            case 4:
                if (input[i] == ':')
                {
                    temp[j] = '\0';
                    tempest.tm_hour = atoi(temp);
                    n = 5;
                    j = 0;
                }
                else
                {
                        temp[j] = input[i];
                        j++;
                }
                break;


                //MINUTE
            case 5:
                if (input[i] == ':')
                {
                    temp[j] = '\0';
                    tempest.tm_min = atoi(temp);
                    n = 6;
                    j = 0;
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;


                //SECOND 
            case 6:
                if (input[i] == ' ')
                {
                    temp[j] = '\0';
                    tempest.tm_sec = atoi(temp);
                    n = 7;
                    j = 0;
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;

            case 7:
                if (input[i] == ']')
                {
                    temp[j] = '\0';
                    //zone = atoi(temp);
                    j = 0;
                }
                else
                {
                    temp[j] = input[i];
                    j++;
                }
                break;         

            default:
                printf("BLARG/n");
                break;
        }

    }
    free(temp);



    /*tempest.tm_hour = hour ;
    tempest.tm_sec = second;
    tempest.tm_min = minute;
    tempest.tm_mday = day;
    tempest.tm_mon = month -1 ;
    tempest.tm_year = year - 1900;
    */

    //printf("tm: %s",asctime(&tempest));
    
    outtime = mktime(&tempest);

    //printf("time_t: %s",asctime(localtime(&outtime))); 


    return outtime;
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
