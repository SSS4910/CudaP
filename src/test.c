// test file


# include <stdio.h>
# include "queue.c"



int main(void)
{
    int MAX_SIZE = 20;
    char *qArray[MAX_SIZE];

    Queue queue;
    queue_init(&queue);

    char *strArray[] = {"one", "two", "three", "bat", "badger", "four", "five"};

    int x;
    for(x = 0; x < 7; x++)
    {
        queue_write(&queue, strArray[x]);
    }

    for(x = 0; x < 7; x++)
    {
        printf("%s\n", queue_read(&queue));
    }

    return 0;
}