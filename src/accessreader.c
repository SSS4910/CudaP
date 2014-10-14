#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "accessreader.h"
#include "debug.h"

void *
access_reader(void * arg){
    char * buffer;
    FILE * logfile;

    errlog_write("Initiating Access Reader thread\n");
    errlog_write("Opening access.log file\n");
    logfile = fopen("access.log", "r");

    errlog_write("Allocating memory for line buffer\n");
    //at some point, we need to make this larger than 4096
    //its a pretty big deal
    buffer = (char *) malloc (4096);

    while (fgets(buffer, 4096, logfile) != NULL)
    {
        printf("%s", buffer);
    }

    //cleanup
    errlog_write("Closing access.log file...\n");
    fclose(logfile);

    errlog_write("Freeing memory for line buffer\n");
    free(buffer);
    
    errlog_write("Exiting Access Reader thread\n");
    pthread_exit(NULL);
}
