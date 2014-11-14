
#ifndef CORE_H
#define CORE_H

#include <time.h>
#include <regex.h>

#define DEBUG 0
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024//327680
#define MAX_LINE_LENGTH 16384

// Structures
typedef struct{
    char *host;
    char *clientId;
    char *userId;
    char *strTime;
    time_t time;
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
    long long int hourlyAccess[24];
    long long int monthlyAccess[12];
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
int log_readline(FILE *, char *, regex_t *);
int delete_output_files();

// Global Variables
extern Buffer buffer1;
extern Buffer buffer2;
extern Statistics totalStats;
extern Queue404 queue404;
extern int MASTER_SWITCH;

#endif
