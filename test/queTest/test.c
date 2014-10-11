// test file


# include <stdio.h>
//# include "queue.h"
# include "queue.c"




int main(void)
{
    int MAX_SIZE = 20;
    char *qArray[MAX_SIZE];

    Queue queue;
    queue_init(&queue);

    char *strArray[] = {"one", "two", "three", "bat", "badger", "four", "five"};    
        printf("test1\n");
        fflush(stdout);
    
    int x;
    for(x = 0; x < 7; x++)
    {
        printf("inloop");
        fflush(stdout);
        queue_write(&queue, strArray[x]);
    }
        printf("test2\n");
        fflush(stdout);

    for(x = 0; x < 7; x++)
    {
        printf("%s\n", queue_read(&queue));
    }

    return 0;
}
