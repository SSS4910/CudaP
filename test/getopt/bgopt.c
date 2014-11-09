#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1


int
main(int argc, char *argv[])
{
    if ((argc != 1) && (argc != 3))
    {
        printf("usage: %s [-f file]", argv[0]);
        return 1;
    }
    return 0;
}
