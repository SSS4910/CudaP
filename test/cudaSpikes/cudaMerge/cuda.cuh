


//#define N 30
#define TRUE 1
#define FALSE 0
#define MAX_BLOCKS 65000
#define COMPUTE_ABILITY 2

typedef struct{
    int available;
    int currentSize;
    struct Request * requests;
} Buffer;

typedef struct{
    char *host;
    char *req;
    char *time;
    int is_injection;
} Struct404;

typedef struct{
    char *host;
    char *req;
    char*time;
} Struct200;

// cuda prototypes
__global__ void analyze_404(Buffer *buffer, Struct404 *results);

__global__ void analyze_200(Buffer *buffer, Struct200 *results);

//__global__ void analyze_height(Buffer *bufer, int *statResults);

int analyze_data(Buffer *input_buffer);

int cuda_setup(int computeCapability);

char * cuda_strcpy(char *dest, const char *source);

int cuda_strcmp(char *str1, char *str2);
