#include <stdio.h>
#include "debug.h"

FILE *debugFile;

/*
 * FUNCTION: open_debug_file
 * ----------------------------
 * creates and opens a file for debugging output
 *
 * returns 0 on successful completion, -1 on failure
 */
int
open_debug_file(){
    debugFile = fopen("errorlog.txt", "w");
    debug_write("~~~ SSS4910 Runtime Log ~~~\n");
    debug_write("For debugging purposes only\n");
    return 0;
}

/*
 * FUNCTION: close_debug_file
 * -----------------------------
 * closes the debugging output file
 *
 * returns 0 on successful completion, -1 on failure
 */
int
close_debug_file(){
    debug_write("~~~ End of execution ~~~\n");
    fclose(debugFile);
    return 0;
}

/*
 * FUNCTION: debug_write
 * ----------------------
 * write a string to the logfile
 *
 * returns 0 on successful completion, -1 on failure
 */
int
debug_write(char *message)
{
    fputs(message, debugFile);
    return 0;
}
