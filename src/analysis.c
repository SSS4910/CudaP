
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "analysis.h"


/*
    FUNCTION: manage_data
    ---------------------
    Monitors the availability of the buffers
    and chooses which buffer to perform analysis on

    @return void*
*/
void * manage_data()
{
    /* 
        Master loop that keeps the thread alive until 
        there is no more data being read in.
        The loop waits/checks for a buffer to
        be ready for reading
    */
    while(MASTER_SWITCH == TRUE)
    {
        if(buffer1.readyRead)
        {
            // Don't analyze empty buffers
            if(buffer1.currentSize != 0)
            {
                if(analyze(&buffer1) == 1)
                {
                    fprintf(stderr, "Analysis Failure\n");
                }

                buffer1.readyRead = FALSE;
                buffer1.readyWrite = TRUE;
            }
        }

        if(buffer2.readyRead)
        {
            // Don't analyze empty buffers
            if(buffer2.currentSize != 0)
            {
                if(analyze(&buffer2) == 1)
                {
                    fprintf(stderr, "Analysis Failure\n");
                }

                buffer2.readyRead = FALSE;
                buffer2.readyWrite = TRUE;
            }
        }
    }

    /* 
        cleanup after master switch is thrown to 
        make sure all data from all buffers has been read
    */
    if(buffer1.readyRead)
    {
        if(buffer1.currentSize != 0)
        {
            if(analyze(&buffer1) == 1)
            {
                fprintf(stderr, "Analysis Failure\n");
            }

            buffer1.readyRead = FALSE;
            buffer1.readyWrite = TRUE;
        }
    }

    if(buffer2.readyRead)
    {
        if(buffer2.currentSize != 0)
        {
            if(analyze(&buffer2) == 1)
            {
                fprintf(stderr, "Analysis Failure\n");
            }

            buffer2.readyRead = FALSE;
            buffer2.readyWrite = TRUE;
        }
    }

    return 0;
}

/*
    FUNCTION: analyze
    -----------------
    Does a linear scan of all elements in buffer
    and performs a series of checks to build
    statistics and structures to help pinpoint
    any potential intrusions

    @param Buffer* buffer to be analyzed
    @return int 0 on success; 1 on failure
*/
int analyze(Buffer *buffer)
{
    int status;

    int total404        = 0;
    int total200        = 0;
    int totalInjections = 0;
    int totalVisits     = 0;

    // Open file to write 404 entries to
    FILE *file404 = fopen("404Data.txt", "a");
    if(file404 == NULL)
    {
        return 1;
    }

    int x;
    for(x = 0; x < buffer->currentSize; x++)
    {

        //Gather time statistics
        struct tm *reqTime;
        reqTime = localtime( &buffer->requests[x].time);
        //printf("time %d\n", reqTime->tm_hour);
        totalStats.hourlyAccess[reqTime->tm_hour]++;
        totalStats.monthlyAccess[reqTime->tm_mon]++;

        // Query URL
        status = query_url(buffer->requests[x].req);
        if(status != 0)
        {
            return 1;
        }

        // check and Handle 200 return codes
        if(buffer->requests[x].retCode == 200)
        {
            total200++;

            if(is_injection(buffer->requests[x].req))
            {
                totalInjections++;
            }
        } // check and handle 404 error codes
        else if(buffer->requests[x].retCode == 404)
        {
            total404++;

            if(is_injection(buffer->requests[x].req))
            {
                totalInjections++;    
            }

            // Add 404 to 404Data file 
            fprintf(file404, "%s;%s;%s;%s;%ld;%s;%d;%d;%s;%s\n", buffer->requests[x].host, 
                                                                 buffer->requests[x].clientId, 
                                                                 buffer->requests[x].userId, 
                                                                 buffer->requests[x].strTime,
                                                                 buffer->requests[x].time,
                                                                 buffer->requests[x].req,
                                                                 buffer->requests[x].retCode,
                                                                 buffer->requests[x].dataSize,
                                                                 buffer->requests[x].referer,
                                                                 buffer->requests[x].userAgent);
            
        }

        totalVisits++;
    }

    fclose(file404);

    #if DEBUG==1
        printf("\nBuffer%d\n", buffer->id);
        printf("Total visits: %d\n", totalVisits);
        printf("Total 200s: %d\n", total200);
        printf("Total 404s: %d\n", total404);
        printf("Total Injections: %d\n", totalInjections);
    #endif

    // add totals to totalStats
    totalStats.total200        += total200;
    totalStats.total404        += total404;
    totalStats.totalInjections += totalInjections;
    totalStats.totalVisits     += totalVisits;

    return 0;

}

/**
    FUNCTION: is_injection
    ----------------------
    Determines if the given c-style string contains 
    any phpmyadmin injections

    @param char* C-style string to be tested
    @return int TRUE (1) or FALSE (0)
*/
int is_injection(char * request)
{
    if(strcmp(request, "/PHPMYADMIN/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/PMA/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/PMA2005/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/SSLMySQLAdmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/SQL/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/admin/phpmyadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/admin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/bbs/data/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpadmindb/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/admin/pma/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpanelmysql/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpanelphpmyadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpanelsql/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/cpphpmyadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/db/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/dbadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/myadmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/mysql-admin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/mysql/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/mysqladmin/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/mysqladminconfig/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    else if(strcmp(request, "/mysqlmanager/scripts/setup.php") == 0)
    {
        return TRUE;
    }
    
    return FALSE;
    
}

int query_url(char *in_url)
{
    int x;
    for(x = 0; x < uniqueRequests.currentSize; x++)
    {
        if(strcmp(in_url, uniqueRequests.urls[x].url) == 0)
        {
            //fprintf(stderr, "In loop: %s\n", in_url);
            uniqueRequests.urls[x].occurances++;
            return 0;
        }
    }

    if(uniqueRequests.currentSize > MAX_UNIQUE_URLS)
    {
        return 1;
    }

    //uniqueRequests.urls[uniqueRequests.currentSize] = (URL*)malloc(sizeof(URL));
    uniqueRequests.urls[uniqueRequests.currentSize].url = (char *)malloc(sizeof(char)*2000);
    strcpy(uniqueRequests.urls[uniqueRequests.currentSize].url, in_url);
    uniqueRequests.urls[uniqueRequests.currentSize].occurances = 1;
    uniqueRequests.currentSize++;


    return 0;

}