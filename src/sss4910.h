#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 2048
#define MAX_LINE_LENGTH 16384

typedef struct{
    char * host;
    char * country;
    char * region;
    //rfc 1413
    //http auth
    //time_t accessTime;
    char * req;
    int httpReturnCode;
    int dataSize;
    char * referer;
    char * userAgent;
} Request;

typedef struct{
    int available;
    int currentSize;
    char ** strArray;
    struct Request * requests;
} Buffer;
