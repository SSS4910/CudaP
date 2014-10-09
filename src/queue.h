

typedef struct{
    int readPtr;
    int writePtr;
    int MAX_SIZE;
    int curSize;
    char* strArray[];
} Queue;

char* queue_read(Queue *queue);
int queue_write(Queue *queue, char * strToWrite);
int is_queue_ready_write(Queue *queue);
int is_queue_ready_read(Queue *queue);
Queue* queue_init(Queue *queue;
int queue_teardown(Queue *queue);