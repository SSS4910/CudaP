


#define N 30
#define TRUE 1
#define FALSE 0
#define MAX_BLOCKS 65000
#define COMPUTE_ABILITY 2

typedef struct{
    int available;
    int currentSize;
    struct Request * requests;
} Buffer;

// cuda prototypes
__global__ void analyze_id(Buffer *buffer, int *statResults);

__global__ void analyze_age(Buffer *buffer, int *statResults);

__global__ void analyze_height(Buffer *bufer, int *statResults);

int analyze_data(Buffer *input_buffer);

int cuda_setup(int computeCapability);
