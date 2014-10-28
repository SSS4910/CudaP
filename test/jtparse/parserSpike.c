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


int parse_line(char* line);

    char* host;   
    char* rfc;
    char* auth;
    char* time;
    char* command; 
    int code;
    int size;
    char* retCode; 
    char* retSize;
    char* referer;
    char* userAgent;
      


int main(int argc, char **argv)
{
    host = (char *)malloc((1000) * sizeof(char));    
    rfc = (char *)malloc((50) * sizeof(char));
    auth = (char *)malloc((150) * sizeof(char));  
    time = (char *)malloc((100) * sizeof(char));  
    command = (char *)malloc((2000) * sizeof(char));  
    code = 0;
    size = 0;
    retCode = (char *)malloc((30) * sizeof(char));  
    retSize = (char *)malloc((30) * sizeof(char));  
    //referer = (char *)malloc((30) * sizeof(char));
    //userAgent = (char *)malloc((30) * sizeof(char)); 
 
    int lineN = 0;



    FILE *fp;
    
    //fp = fopen("bgcsierravlanca_access.log","r");
    //fp = fopen("almhuette_access.log", "r");
    fp = fopen("UofS_access_log","r");
    //fp = fopen("redlug.log","r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    if (fp == NULL) 
    {
        printf("Im sorry invalid file");
        exit(EXIT_FAILURE);
    }
    while ((read = getline(&line, &len, fp)) != -1)
    {
        //printf("    * * %s \n",line);
        parse_line(line);
       // printf("\n %d \n", lineN);
        lineN++;
    }
    printf("%d    %s \n",lineN,line);
    printf("DONE :D\n");
   

    //printf("H:%s R:%s A:%s T:%s C:%s c:%d S:%d \n",host,rfc, auth, time, command, code, size );
    

    
    
        //CRASHING HERE!
    free(host);
    free(rfc);
    free(auth);
    free(time);
    free(command);
    free(retCode);
    free(retSize);
    //free(referer);
    //free(userAgent);
    fclose(fp);
}


int parse_line(char* line)
{
    //printf("%s \n",line);
    int n = 0;
    int j = 0;
    int i = 0;
    int h = 0;
    int k = 0;
  
    /*char* host;
    host = (char *)malloc((30) * sizeof(char));    
    
    char* rfc;
    rfc = (char *)malloc((30) * sizeof(char)); 

    char* auth;
    auth = (char *)malloc((30) * sizeof(char));  
    
    char* time;
    time = (char *)malloc((30) * sizeof(char));  
    
    char* command;
    command = (char *)malloc((100) * sizeof(char));  
    
    int code = 0;
    int size = 0;

    char* retCode;
    retCode = (char *)malloc((30) * sizeof(char));  
    
    char* retSize;
    retSize = (char *)malloc((30) * sizeof(char));  
    
    char* referer;
    referer = (char *)malloc((30) * sizeof(char));  
    
    char* userAgent;
    userAgent = (char *)malloc((30) * sizeof(char));*/  
   
    int flag = 0;
    
    for(i =0; i < strlen(line); i++)
    {
        switch(n)
        {
            case 0:
                if (line[i] == ' ')
                {
                    n = 1;
                    host[j] = '\0';
                    j =0;
                }
                else
                {
                    host[j] = line[i];
                    j++;
                }
                break;
            case 1:
                if(line[i] == ' ')
                {
                    n = 2;
                    rfc[j] = '\0';
                    j =0;
                }
                else
                {
                    rfc[j] = line[i];
                    j++;
                }
                break;
            case 2:
                if(line[i] == ' ')
                {
                    n = 3;
                    auth[j] = '\0';
                    j = 0;
                }
                else
                {
                    auth[j] = line[i];
                    j++;
                }
                break;
            case 3:
                if(flag == 1)
                {
                    n = 4;
                    flag = 0;
                    time[j] = '\0';
                    j = 0;

                }
                else if(line[i] == ']')
                {
                    flag = 1;
                    time[j] = line[i];
                    j++;
                }
                else
                {
                    time[j] = line[i];
                    j++;
                }
                break;
           
            case 4:
                if(flag == 2)
                {
                    if (line[i] != ' ')
                    {
                        flag = 1;
                        n = 4;
                        command[j] = line[i];
                        j++;
                    }
                    else
                    {
                        flag = 0;
                        n = 5;
                        h = 0;
                        command[j] = '\0';
                    }
                }
                else if( line[i] == '"')
                {
                    command[j] = line[i];
                    j++;
                    flag++;
                }
                else
                {
                    command[j] = line[i];
                    j++;
                }
                break;
            case 5:
                if(line[i] == ' ')
                {
                    retCode[h] = '\0';
                    code = atoi(retCode);
                    if(code == 0)
                    {
                        //n = 5;
                        for(k = 0; k < h; k++)
                        {
                            command[j] = retCode[k];
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
            case 6:
                if(line[i] == ' ')
                {
                    n = 7;
                    retSize[h] = '\0';
                    h = 0;
                    size = atoi(retSize);
                }
                else
                {
                    retSize[h] = line[i];
                    h++;
                }
                break;

        }   


    }

        //this is going to need to get moved
    size = atoi(retSize);
    
    //printf("H:%s R:%s A:%s T:%s C:%s c:%d S:%d \n",host,rfc, auth, time, command, code, size );
    

    //return struct here


    /*free(host);
    free(rfc);
    free(auth);
    free(time);
    free(command);
    free(retCode);
    free(retSize);
    free(referer);
    free(userAgent);*/

}
