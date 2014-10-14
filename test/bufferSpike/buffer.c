/*
 *          buffer.c
 *
 *          Description:
 */

# include <stdio.h>
# include <string.h>

typedef stuct {
    int size;
    int ready;
    char* strArray[];
} Buffer;

