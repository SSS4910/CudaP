#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../../src/sss4910.h"
#include "../../src/debug.h"
#include "../../src/ppfile.h"
#include "../../src/parser.h"

char *
slice(char *, int, int);
/*
int main(int argc, char **argv){
    char * longstring = "letter is a six letter word";
    char * s = slice(longstring, 0, 6);
    printf("original string: \"%s\"\n", longstring);
    printf("substring: \"%s\"\n", s);
    return 0;
}
*/

int
main(int argc, char **argv){
    int field = 0;
    int start = 0;
    int end = 0;
    int ignorewhitespace = FALSE;
    int size;
    char *substring;
    char *line = "field0 field1 field2 \"field3 a b\"\n";
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        //printf("line length: %d\n", strlen(line));
        if ((line[i] != ' ') && (line[i] != '\"'))
        {
            end++;
        }
        if (((line[i] == ' ') || line[i] == '\n') && (ignorewhitespace == FALSE))
        {
            size = end - start;
            substring = (char *) malloc(size * sizeof(char));
            switch(field)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    substring = slice(line, start, end);
                    printf("substring: %s\n", substring);
                    break;
                default:
                    //printf("extra field detected!");
                    break;
            }
            start = end + 1;
            field++;
        }
        if ((line[i] == '\"') && (ignorewhitespace == FALSE))
        {
            //start ignoring whitespace characters
            //printf("<ignoring whitespace characters!>");
            ignorewhitespace = TRUE;
            continue;
        }
        if ((line[i] == '\"') && (ignorewhitespace == TRUE))
        {
            //stop ignoring whitespace
            //printf("<done ignoring whitespace!>");
            ignorewhitespace = FALSE;
            continue;
        }
    }
    printf("Original Line: %s", line);
    return 0;
}

/* FUNCTION: slice
 * ---------------
 * takes a substring of the provided string from start index to end index
 *
 * string: a string to generate a substrings
 * start: start index (inclusive)
 * end: end index (inclusive)
 *
 * return: string
 */
char *
slice(char *string, int start, int end){
    int i;
    int j = start;
    int size = end - start;
    char *substring = (char *)malloc((size) * sizeof(char));
    for (i = 0; i <= size; i++)
    {
        substring[i] = string[j];
        j++;
    }

    //substring[size] = '\0';
    return substring;
}
