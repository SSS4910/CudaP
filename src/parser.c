#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sss4910.h"
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
parse_line(char * line, Request *request){
    return 0;
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
