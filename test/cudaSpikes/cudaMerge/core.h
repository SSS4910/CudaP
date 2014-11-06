
#ifndef CORE_H
#define CORE_H

#include <time.h>
#include <stdio.h>
/*#include "cuda.cuh"
#include "parser.h"
#include "debug.h"*/

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 15 //327680
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

int
buffer_init(Buffer *);

void
buffer_free(Buffer *);

int
req_init(Request *);

int
req_null(Request *);

void
req_free(Request *);

char *
log_readline(FILE *);

#endif
