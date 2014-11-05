#include <time.h>

#define DEBUG 1
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 327680
#define MAX_LINE_LENGTH 16384

typedef struct{
    char *host;
    char *clientId;
    char *userId;
    char *time;
    char *req;
    int retCode;
    int dataSize;
    char *referer;
    char *userAgent;
} Request;

typedef struct{
    int available;
    int currentSize;
    Request * requests;
} Buffer;
