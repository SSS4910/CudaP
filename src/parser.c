#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int so = 0;
    int eo = so;
    int field = 0;
    int ignorespace = FALSE;
    Request * request = malloc(sizeof (Request *));
    printf("%s", line);
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        switch (line[i])
        {
            case '\"':
                printf("\"");
                if (ignorespace) { ignorespace = FALSE; }
                else { ignorespace = TRUE; }
                break;
            case '[':
                ignorespace = TRUE;
                printf("[");
                break;
            case ']':
                ignorespace = FALSE;
                printf("]");
                break;
            case '\n':
            case ' ':
                if (line[i] == '\n')
                {
                    printf("!");
                    field++;
                    break;
                }
                if (ignorespace)
                {
                    printf("*");
                }
                else
                {
                    printf(" ");
                    field++;
                }
                break;
            default:
                printf("c");
                eo++;
                break;
        }
    }
    printf("\n");
    printf("SOME STATISTICS:\n\t%d fields\n", field);
    return request;
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
