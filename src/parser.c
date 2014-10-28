#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "sss4910.h"
#include "parser.h"
#include "debug.h"

/*
 * FUNCTION: parse
 * --------------
 * parses a NCSA Common Log Format string into a Request structure
 *
 * line: a string containing a NCSA Common Log Format access line
 *
 * returns:
 *  pointer to a Request structure on success
 *  null on failure
 */
Request *
parse(char * line){
    Request * request = malloc(sizeof (Request *));
    printf("%s", line);
    return request;
}

/*
 * FUNCTION: slice
 * ---------------
 * returns a slice of given string from start offset to end offset
 *
 * line: a char pointer to be sliced
 * so: the start offset
 * eo: the end offset
 */
char *
slice(char *line, int so, int eo){
    char *slice = NULL;
    return slice;
}
