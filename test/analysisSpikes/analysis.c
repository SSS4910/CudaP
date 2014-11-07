
#include <stdio.h>
#include <string.h>
#include "analysis.h"


/*
    Monitors the availability of the buffers
    and chooses which buffer to perform analysis on
*/
void manage_data()
{
    /* 
        Master loop that keeps the thread alive until 
        there is no more data being read in.
        The loop waits/checks for a buffer to
        be reading for reading
    */
    while(MASTER_SWITCH)
    {
        if(buffer1.readyRead)
        {
            analyze(&buffer1);

            buffer1.readyRead = FALSE;
            buffer1.readyWrite = TRUE;
        }

        if(buffer2.readyRead)
        {
            analyze(&buffer2);

            buffer2.readyRead = FALSE;
            buffer2.readyWrite = TRUE;
        }
    }

    /* 
        cleanup after master switch is thrown to 
        make sure all data from all buffers has been read
    */
    if(buffer1.readyRead)
    {
        analyze(&buffer1);

        buffer1.readyRead = FALSE;
        buffer1.readyWrite = TRUE;
    }

    if(buffer2.readyRead)
    {
        analyze(&buffer2);

        buffer2.readyRead = FALSE;
        buffer2.readyWrite = TRUE;
    }
}

/*
    Does a linear scan of all elements in buffer
    and performs a series of checks to build
    statistics and structures to help pinpoint
    any potential intrusions

    INPUT: Buffer *
    OUTPUT: int (bool) success/failure
*/
int analyze(Buffer *buffer)
{
    int total404        = 0;
    int total200        = 0;
    int totalInjections = 0;
    int totalVisits     = 0;

    int x;
    for(x = 0; x < buffer->currentSize; x++)
    {
        // check and Handle 200 return codes
        if(buffer->requests[x].retCode == 200)
        {
            total200++;

            if(is_injection(buffer->requests[x].req))
            {
                totalInjections++;
            }
        } // check and handle 400 error codes
        else if(buffer->requests[x].retCode == 404)
        {
            total404++;

            if(is_injection(buffer->requests[x].req))
            {
                totalInjections++;
            }
        }

        totalVisits++;
    }

    return TRUE;
}

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