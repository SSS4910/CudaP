

#include <stdio.h>
#include "cuda.cuh"

int main(void)
{
    char *thing1 = "w8";
    char *thing2 = "w8";

    printf("Length of thing1: %d\n", strlen(thing1));

    int result;
    result = cuda_strcmp(thing1, thing2);

    printf("Compare result: %d\n", result);

    thing2 = cuda_strcpy(thing2, thing1);

    printf("Copy result: %s\n", thing2);


}