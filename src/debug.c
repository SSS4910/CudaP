#include <stdio.h>
#include "debug.h"

FILE* errorLogFile;

/*
 * Returns 0 on success, -1 on failure
 */
int
open_error_logfile(){
    errorLogFile = fopen("errorlog.txt", "w");
    errlog_write("~~~ SSS4910 Runtime Log ~~~\n");
    errlog_write("For debugging purposes only\n");
    return 0;
}

/*
 * Returns 0 on success, -1 on failure
 */
int
close_error_logfile(){
    errlog_write("~~~ End of execution ~~~\n");
    fclose(errorLogFile);
    return 0;
}


/*
 * Returns number of bytes written to file
 */
int
errlog_write(char* message)
{
    fputs(message, errorLogFile);
    return 0;
}
