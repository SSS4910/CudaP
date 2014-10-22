#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../../src/sss4910.h"
#include "../../src/debug.h"
#include "../../src/outputreq.h"
#include "../../src/parser.h"

int
main(int argc, char **argv){
    int i;
    int start = 0;
    int end = 0;
    int field = 0;
    int ignorewhitespace = FALSE;
    const char *line = "field0 field1 field2 \"field3 extra stuff\" field4 field5\n";
    for (i = 0; i < strlen(line); i++)
    {
        end++;
        if ((line[i] == '\"') && (ignorewhitespace == FALSE))
        {
            //start ignoring whitespace characters
            printf("<ignoring whitespace characters!>");
            ignorewhitespace = TRUE;
            continue;
        }
        if ((line[i] == '\"') && (ignorewhitespace == TRUE))
        {
            //stop ignoring whitespace
            printf("<done ignoring whitespace!>");
            ignorewhitespace = FALSE;
            continue;
        }
        if (((line[i] == ' ') || line[i] == '\n') && (ignorewhitespace == FALSE))
        {
            switch(field)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:

                    printf("field%d start:%d end:%d\n", field, start, end);
                    break;
                default:
                    printf("extra field detected!");
                    break;
            }
            start = end;
            field++;
        }
    }
    printf("Original Line: %s", line);
    return 0;
}
