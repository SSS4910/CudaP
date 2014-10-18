#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 512
#define MAX_LINE_LENGTH 4096  

typedef struct{
    int available;
    char ** strArray; 
} Buffer;
