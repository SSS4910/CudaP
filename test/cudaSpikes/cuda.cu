
#include "cuda.cuh"
#include "book.h"


// Declare the Cuda kernels and any Cuda functions
__global__ void analyze_id(Buffer *buffer, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {

    }
    
}

__global__ void analyze_age(Buffer *buffer, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {

    }

}

__global__ void analyze_height(Buffer *buffer, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {

    }
    
}

int analyze_data(Buffer *input_buffer)
{
    int threads = N;
    int blocks = threads/2; // to be more conservative w/blocks: (N + (threads - 1)) / threads
    
    if(blocks > MAX_BLOCKS)
    {
        printf("Data set is too large\nSet cannot exceed %d elements\n", MAX_BLOCKS);
        exit(1);
    }

    cuda_setup(COMPUTE_ABILITY);

    // CPU variables
    Buffer *cudaBuffer;
    int idStats[N];
    int ageStats[N];
    int heightStats[N];

    // GPU variables
    Buffer *dev_buffer;
    int *dev_idStats;
    int *dev_ageStats;
    int *dev_heightStats;

    // events to track performance time
    float elapsedTime;
    cudaEvent_t start, stop;
    HANDLE_ERROR(cudaEventCreate(&start));
    HANDLE_ERROR(cudaEventCreate(&stop));
    HANDLE_ERROR(cudaEventRecord(start, 0));

    // creates zero-copy memory for buffer (both CPU and GPU point to same memory). A pointer will be given to the GPU later...
    HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer, sizeof(input_buffer), cudaHostAllocWriteCombined | cudaHostAllocMapped));

    // fill the zero-copy memorywith data from input_buffer
    cudaBuffer = (Buffer *)memcpy(&cudaBuffer, &input_buffer, sizeof(input_buffer));

    // lets main know the buffer is free
    input_buffer->available = TRUE;

    //Declare streams
    cudaStream_t stream0;
    cudaStream_t stream1;
    cudaStream_t stream2;

    //create steams
    HANDLE_ERROR(cudaStreamCreate(&stream0));
    HANDLE_ERROR(cudaStreamCreate(&stream1));
    HANDLE_ERROR(cudaStreamCreate(&stream2));

    // allocating GPU memory (GPU only memory)
    HANDLE_ERROR(cudaMalloc( (void **) &dev_idStats, N * sizeof(int) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_ageStats, N * sizeof(int) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_heightStats, N * sizeof(int) ));

    // page-locking output buffers (pin host memory for streams)
    HANDLE_ERROR(cudaHostAlloc( (void **) &idStats, N * sizeof(int), cudaHostAllocDefault));
    HANDLE_ERROR(cudaHostAlloc( (void **) &ageStats, N * sizeof(int), cudaHostAllocDefault));
    HANDLE_ERROR(cudaHostAlloc( (void **) &heightStats, N * sizeof(int), cudaHostAllocDefault));

    /* FILL BUFFER WITH DATA */

    // gives a pointer to the GPU to reference the zero-copy memory
    HANDLE_ERROR(cudaHostGetDevicePointer(&dev_buffer, cudaBuffer, 0));

    // calls to Cuda kernels, note streams have been added
    analyze_id<<< blocks, threads, 0, stream0 >>>(dev_buffer, dev_idStats);
    analyze_age<<< blocks, threads, 0, stream1 >>>(dev_buffer, dev_ageStats);
    analyze_height<<< blocks, threads, 0, stream2 >>>(dev_buffer, dev_heightStats);

    // Get the results from the GPU
    HANDLE_ERROR(cudaMemcpyAsync(idStats, dev_idStats, N * sizeof(int), cudaMemcpyDeviceToHost, stream0));
    HANDLE_ERROR(cudaMemcpyAsync(ageStats, dev_ageStats, N * sizeof(int), cudaMemcpyDeviceToHost, stream1));
    HANDLE_ERROR(cudaMemcpyAsync(heightStats, dev_heightStats, N * sizeof(int), cudaMemcpyDeviceToHost, stream2));

    // make sure everyone is done
    HANDLE_ERROR(cudaStreamSynchronize(stream0));
    HANDLE_ERROR(cudaStreamSynchronize(stream1));
    HANDLE_ERROR(cudaStreamSynchronize(stream2));

    HANDLE_ERROR(cudaThreadSynchronize());

    //stop timing events
    HANDLE_ERROR(cudaEventRecord(stop, 0));
    HANDLE_ERROR(cudaEventSynchronize(stop));
    HANDLE_ERROR(cudaEventElapsedTime(&elapsedTime, start, stop));

    // cuda cleanup
    cudaFree(dev_buffer);
    cudaFree(dev_idStats);
    cudaFree(dev_ageStats);
    cudaFree(dev_heightStats);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaStreamDestroy(stream0);
    cudaStreamDestroy(stream1);
    cudaStreamDestroy(stream2);

    // total results
    int idTotal = 0;
    int ageTotal = 0;
    int heightTotal = 0;

    int x = 0; // loop iterator

    // id totals
    for(x = 0; x < N; x++)
    {
        idTotal += idStats[x];
    }

    // age totals
    for(x = 0; x < N; x++)
    {
        ageTotal += ageStats[x];
    }

    // height totals
    for(x = 0; x < N; x++)
    {
        heightTotal += heightStats[x];
    }

    // print final totals
    printf("\n");
    printf("ID total: %d\n", idTotal);
    printf("Age total: %d\n", ageTotal);
    printf("Height total: %d\n", heightTotal);
    printf("\n");
    printf("blocks used: %d\n", blocks);
    printf("threads used: %d\n", threads);
    printf("threads per block: %d\n", (threads/blocks));
    printf("\n");
    printf("Elapsed time: %3.1f ms\n", elapsedTime);
    printf("\n");
    printf("End of cuda struct test\n");


    return 0;
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