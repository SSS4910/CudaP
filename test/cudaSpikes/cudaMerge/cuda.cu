
#include "cuda.cuh"
#include "book.h"


// Declare the Cuda kernels and any Cuda functions

/*
    Searches the Buffer for requests that resulted in a 404.
    Upon finding a 404, a stucture is made out of the request
    containing: who sent the request, when was the request made, what was the request, and if the request was a phpmyadmin injection 
*/
__global__ void analyze_200(char **buffer, Struct200 *results, int *stats)
{ // buffer->requests[id].thing
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < 5)
    {
        printf("GPU: %s\n", buffer[id]);
        stats[id] = id;
    }
}
 

/*
    Takes a Buffer and uses multiple Cuda kernels,
    launched asynchronously, to perform multiple 
    kinds of analysis and statistics on the Buffer's
    contents. Most test will focus on finding potential 
    security intrusions.

    INPUT: Buffer*
    OUTPUT: int, true or false, to determine if analysis was successful
*/
int analyze_data(Buffer *input_buffer)
{
    // localizing buffer values since they will no longer be reliable 
    // after input_buffer is available
    int N = input_buffer->currentSize;
    unsigned int buffer_size = deep_sizeof(input_buffer);//sizeof(input_buffer);//1264696+8;//sizeof(input_buffer);//sizeof(Buffer) * N;//(72+4360)*N;//sizeof(input_buffer->requests[0]);

//############# CPU TESTS ##################################
    int cpuTotal200 = 0;
    int c;
    for(c = 0; c < N; c++)
    {
        if(input_buffer->requests[c].retCode == 200)
            cpuTotal200++;
    }
//##########################################################

    printf("N (input_buffer->currentSize) = %d\n", N);
    printf("buffer_size = %d\n", buffer_size);

    int threads = N;
    int blocks = threads/2; // to be more conservative w/blocks: (N + (threads - 1)) / threads
    fprintf(stderr, "Num threads: %d\n", threads);
    fprintf(stderr, "Num blocks: %d\n", blocks);

    if(blocks > MAX_BLOCKS)
    {
        printf("Data set is too large\nSet cannot exceed %d elements\n", MAX_BLOCKS);
        return FALSE;
    }

    

    // CPU variables
    //Request *cudaBuffer;// = (Buffer *)malloc(sizeof(Buffer));
    char *cudaBuffer[] = {"one", "two", "three", "four", "five"};
    Struct200 results200[N];
    int stats200[N];

    // GPU variables
    //Request *dev_buffer;
    char **dev_buffer;
    Struct200 *dev_results200;
    int *dev_stats200;

    // creates zero-copy memory for buffer (both CPU and GPU point to same memory). A pointer will be given to the GPU later...
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer, sizeof(cudaBuffer), cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer[0], 4, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer[1], 4, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer[2], 5, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer[3], 5, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer[4], 5, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    


    /*int i;
    for(i = 0; i < N; i++)
    {
        cuda_req_init(&cudaBuffer[i]);
    }*/

    /*memcpy(cudaBuffer, input_buffer->requests, sizeof(Request) * N);
    int j;
    for(j = 0; j < N; j++)
    {
        printf("CPU-Host: %s\tCPU-Return Code: %d\n", cudaBuffer[j].host, cudaBuffer[j].retCode);
    }*/

    // allocating GPU memory (GPU only memory)
    HANDLE_ERROR(cudaMalloc( (void **) &dev_results200, N * sizeof(Struct200) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_stats200, N * sizeof(int) ));

    // page-locking output buffers (pin host memory for streams)
    /*HANDLE_ERROR(cudaHostAlloc( (void **) &results200, N * sizeof(Struct200), cudaHostAllocDefault));
    HANDLE_ERROR(cudaHostAlloc( (void **) &stats200, N * sizeof(int), cudaHostAllocDefault));*/

    /* FILL BUFFER WITH DATA */

    // gives a pointer to the GPU to reference the zero-copy memory
    HANDLE_ERROR(cudaHostGetDevicePointer(&dev_buffer, *cudaBuffer, 0));

    // calls to Cuda kernels, note streams have been added
    analyze_200<<< blocks, threads >>>(dev_buffer, dev_results200, dev_stats200);

    // Get the results from the GPU
    HANDLE_ERROR(cudaMemcpy(results200, dev_results200, N * sizeof(Struct200), cudaMemcpyDeviceToHost));
    HANDLE_ERROR(cudaMemcpy(stats200, dev_stats200, N * sizeof(int), cudaMemcpyDeviceToHost));

    HANDLE_ERROR(cudaThreadSynchronize());

    /* Print output for testing */
    int total200 = 0;

    int x;
    for(x = 0; x < N; x++)
    {
        total200 += stats200[x];

        fprintf(stderr, "200[%d] = %d\n", x, stats200[x]);
    }

    printf("CPU total 200: %d\n", cpuTotal200);
    printf("Total 200s: %d\n", total200);

    // cuda cleanup
    cudaFree(dev_buffer);
    cudaFree(dev_results200);
    cudaFree(dev_stats200);

    cudaFree(cudaBuffer); // zero-copy

    /* Send results to files?? */

    return TRUE;
}

int cuda_setup(int computeCapability)
{
    int dev;
    //int numOfDevs;

    cudaDeviceProp prop;

    // assign number of cuda devices to 3rd element in specs array
    //HANDLE_ERROR(cudaGetDeviceCount(&numOfDevs));

    // gets current device
    HANDLE_ERROR(cudaGetDevice(&dev));

    // create a "pseudo" device w/ desired values and let cuda api pick device that matches
    // memset is just zero-ing out the specified memory
    memset(&prop, 0, sizeof(cudaDeviceProp));
    prop.major = computeCapability;
    HANDLE_ERROR(cudaChooseDevice(&dev, &prop));

    // sets the device w/ the desired paramaters as the device to use
    HANDLE_ERROR(cudaSetDevice(dev));

    // allow GPU to zero-copy memory (map)
    HANDLE_ERROR(cudaSetDeviceFlags(cudaDeviceMapHost));

    // get more info about the device
    //HANDLE_ERROR(cudaGetDeviceProperties(&prop, dev));

    return 0;
}

// Copies strings in cuda code
// Returns: pointer to dest
char * cuda_strcpy(char *dest, const char *source)
{
    int length = strlen(source);
    dest = (char *) malloc(length + 1);

    int x = 0;
    for(x = 0; x < length; x++)
    {
        dest[x] = source[x];
    }
    dest[length + 1] = '\0';

    return dest;

    
}

// Compares strings in cuda code
// Returns: 0 if equal, -1 if not
int cuda_strcmp(char *str1, char *str2)
{
    unsigned int str1Len = sizeof(str1);
    unsigned int str2Len = sizeof(str2);

    if(str1Len != str2Len)
    {
        return -1;
    }
    else
    {
        int x;
        for(x = 0; x < str1Len; x++)
        {
            if(str1[x] != str2[x])
            {
                return -1;
            }
        }
    }

    return 0;

}

unsigned int deep_sizeof(Buffer *buffer)
{
    unsigned int size = 0;
    size += sizeof(Buffer);
    size += sizeof(Request) * buffer->currentSize;

    int x;
    for(x = 0; x < buffer->currentSize; x++)
    {
        size += sizeof(buffer->requests[x]);
    }

    return size;
}

// Performs Zero-Copy
int cuda_buffer_init(Buffer * buffer){
    int i;
    //buffer->requests = (Request *)malloc((BUFFER_SIZE) * sizeof(Request));
    HANDLE_ERROR( cudaHostAlloc( (void **) &buffer, sizeof(Buffer), cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &buffer->requests, sizeof(Request) * BUFFER_SIZE, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (cuda_req_init(&buffer->requests[i]))
        {
            //debug_write("request struct allocation error!\n");
            return 1;
        }
    }
    return 0;
}

int
cuda_req_init(Request *request)
{

    HANDLE_ERROR( cudaHostAlloc( (void **) &request->host, sizeof(char) * 1000, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->clientId, sizeof(char) * 50, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->userId, sizeof(char) * 150, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->time, sizeof(char) * 100, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->req, sizeof(char) * 2000, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->referer, sizeof(char) * 30, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &request->userAgent, sizeof(char) * 30, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    //HANDLE_ERROR( cudaHostAlloc( (void **) &request->retCode, sizeof(int), cudaHostAllocWriteCombined | cudaHostAllocMapped));
    //HANDLE_ERROR( cudaHostAlloc( (void **) &request->dataSize, sizeof(int), cudaHostAllocWriteCombined | cudaHostAllocMapped));

    return 0;
}