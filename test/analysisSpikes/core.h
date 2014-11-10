
#ifndef CORE_H
#define CORE_H

#include <time.h>

#define DEBUG 1
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024//327680
#define MAX_LINE_LENGTH 25000

// Structures
typedef struct{
    char *host;
    char *clientId;
    char *userId;
    char *time;
    //time_t time;
    char *req;
    int retCode;
    int dataSize;
    char *referer;
    char *userAgent;
} Request;

typedef struct{
    int id;
    int readyRead;
    int readyWrite;
    int currentSize;
    Request * requests;
} Buffer;

typedef struct{
    int total200;
    int total404;
    int totalInjections;
    int totalVisits;
} Statistics;

typedef struct{
    int currentIndex;
    int currentSize;
    Request *requests;
} Queue404;

// Function Prototypes
int buffer_init(Buffer *);
void buffer_free(Buffer *);
int req_init(Request *);
int req_null(Request *);
void req_free(Request *);
char * log_readline(FILE *);

// Global Variables
extern Buffer buffer1;
extern Buffer buffer2;
extern Statistics totalStats;
extern Queue404 queue404;
extern int MASTER_SWITCH;

#endif
