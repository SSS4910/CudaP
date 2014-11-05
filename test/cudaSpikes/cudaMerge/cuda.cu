
#include "cuda.cuh"
#include "book.h"


// Declare the Cuda kernels and any Cuda functions

/*
    Searches the Buffer for requests that resulted in a 404.
    Upon finding a 404, a stucture is made out of the request
    containing: who sent the request, when was the request made, what was the request, and if the request was a phpmyadmin injection 
*/
__global__ void analyze_404(Buffer *buffer, Struct404 *results, int *stats)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < 15)//buffer->currentSize)
    {
        if( buffer->requests[id].retCode == 404)
        {
            stats[id] = 1;
           /* cuda_strcpy(results[id].host, buffer->requests[id]->host);
            cuda_strcpy(results[id].req, buffer->requests[id]->req);
            cuda_strcpy(results[id].time, buffer->requests[id]->time);

            if(cuda_strcmp(buffer->requests[id]->req, "/PHPMYADMIN/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/PMA/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/PMA2005/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/SSLMySQLAdmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/SQL/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/admin/phpmyadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/admin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/bbs/data/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpadmindb/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/admin/pma/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpanelmysql/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpanelphpmyadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpanelsql/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/cpphpmyadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/db/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/dbadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/myadmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/mysql-admin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/mysql/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/mysqladmin/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/mysqladminconfig/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else if(cuda_strcmp(buffer->requests[id]->req, "/mysqlmanager/scripts/setup.php") == 0)
            {
                results[id].is_injection = TRUE;
            }
            else
            {
                results[id].is_injection = FALSE;
            }

            //if(results[id].is_injection)
           // {
                stats[id] = 1;
            //}*/

        }
        else
        {
            stats[id] = 0;
        }
    }
    else
    {
        stats[id] = buffer->requests[id].retCode;
    }
    
}

__global__ void analyze_200(Buffer *buffer, Struct200 *results, int *stats)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < 15)//buffer->currentSize)
    {
        if(buffer->requests[id].retCode == 200)
        {
            stats[id] = 1;
        }
        else
        {
            stats[id] = 0;
        }
    }
    else
    {
        stats[id] = buffer->requests[id].retCode;
    }
}

/*__global__ void analyze_height(Buffer *buffer, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {

    }
    
}*/

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
    unsigned int buffer_size = 1264696+8;//sizeof(input_buffer);//sizeof(Buffer) * N;//(72+4360)*N;//sizeof(input_buffer->requests[0]);

    int cpuTotal404 = 0;
    int cpuTotal200 = 0;
    fprintf(stderr, "Calculating CPU totals...\n");
    int c;
    for(c = 0; c < N; c++)
    {
        if(input_buffer->requests[c].retCode == 404)
            cpuTotal404++;

        if(input_buffer->requests[c].retCode == 200)
            cpuTotal200++;
    }

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

    //cuda_setup(COMPUTE_ABILITY);

    // CPU variables
    //Buffer *cudaBuffer;
    Struct404 results404[N];
    Struct200 results200[N];
    int stats404[N];
    int stats200[N];

    // GPU variables
    Buffer *dev_buffer;
    Struct404 *dev_results404;
    Struct200 *dev_results200;
    int *dev_stats404;
    int *dev_stats200;

    // events to track performance time
    float elapsedTime;
    cudaEvent_t start, stop;
    HANDLE_ERROR(cudaEventCreate(&start));
    HANDLE_ERROR(cudaEventCreate(&stop));
    HANDLE_ERROR(cudaEventRecord(start, 0));

    fprintf(stderr, "Before zero-copy\n" );

    // creates zero-copy memory for buffer (both CPU and GPU point to same memory). A pointer will be given to the GPU later...
    //HANDLE_ERROR( cudaHostAlloc( (void **) &cudaBuffer, buffer_size, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    HANDLE_ERROR( cudaHostAlloc( (void **) &input_buffer, buffer_size, cudaHostAllocWriteCombined | cudaHostAllocMapped));
    fprintf(stderr, "After zero-copy\n" );

    // fill the zero-copy memory with data from input_buffer
    //memcpy(&cudaBuffer, &input_buffer, buffer_size);
    //fprintf(stderr, "After buffer memcpy\n" );
    // lets main know the buffer is free
    //input_buffer->available = TRUE;

    //Declare streams
    cudaStream_t stream0;
    cudaStream_t stream1;
    //cudaStream_t stream2;

    //create steams
    HANDLE_ERROR(cudaStreamCreate(&stream0));
    HANDLE_ERROR(cudaStreamCreate(&stream1));
    //HANDLE_ERROR(cudaStreamCreate(&stream2));

    // allocating GPU memory (GPU only memory)
    HANDLE_ERROR(cudaMalloc( (void **) &dev_results404, N * sizeof(Struct404) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_stats404, N * sizeof(int) ));

    HANDLE_ERROR(cudaMalloc( (void **) &dev_results200, N * sizeof(Struct200) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_stats200, N * sizeof(int) ));

    // page-locking output buffers (pin host memory for streams)
    HANDLE_ERROR(cudaHostAlloc( (void **) &results404, N * sizeof(Struct404), cudaHostAllocDefault));
    HANDLE_ERROR(cudaHostAlloc( (void **) &stats404, N * sizeof(int), cudaHostAllocDefault));

    HANDLE_ERROR(cudaHostAlloc( (void **) &results200, N * sizeof(Struct200), cudaHostAllocDefault));
    HANDLE_ERROR(cudaHostAlloc( (void **) &stats200, N * sizeof(int), cudaHostAllocDefault));

    /* FILL BUFFER WITH DATA */

    // gives a pointer to the GPU to reference the zero-copy memory
    //HANDLE_ERROR(cudaHostGetDevicePointer(&dev_buffer, cudaBuffer, 0));
    HANDLE_ERROR(cudaHostGetDevicePointer(&dev_buffer, input_buffer, 0));

    // calls to Cuda kernels, note streams have been added
    analyze_404<<< blocks, threads, 0, stream0 >>>(dev_buffer, dev_results404, dev_stats404);
    analyze_200<<< blocks, threads, 0, stream1 >>>(dev_buffer, dev_results200, dev_stats200);
    //analyze_height<<< blocks, threads, 0, stream2 >>>(dev_buffer, dev_heightStats);

    // Get the results from the GPU
    HANDLE_ERROR(cudaMemcpyAsync(results404, dev_results404, N * sizeof(Struct404), cudaMemcpyDeviceToHost, stream0));
    HANDLE_ERROR(cudaMemcpyAsync(stats404, dev_stats404, N * sizeof(int), cudaMemcpyDeviceToHost, stream0));

    HANDLE_ERROR(cudaMemcpyAsync(results200, dev_results200, N * sizeof(Struct200), cudaMemcpyDeviceToHost, stream1));
    HANDLE_ERROR(cudaMemcpyAsync(stats200, dev_stats200, N * sizeof(int), cudaMemcpyDeviceToHost, stream1));

    // make sure everyone is done
    HANDLE_ERROR(cudaStreamSynchronize(stream0));
    HANDLE_ERROR(cudaStreamSynchronize(stream1));
    //HANDLE_ERROR(cudaStreamSynchronize(stream2));

    HANDLE_ERROR(cudaThreadSynchronize());

    //stop timing events
    HANDLE_ERROR(cudaEventRecord(stop, 0));
    HANDLE_ERROR(cudaEventSynchronize(stop));
    HANDLE_ERROR(cudaEventElapsedTime(&elapsedTime, start, stop));

    /* Print output for testing */
    int total404 = 0;
    int total200 = 0;
    
    /*int x = 0;
    for(x = 0; x < N; x++)
    {
        if(input_buffer->requests[x].retCode == 404)
        {
            total404++;
            fprintf(stderr, "    %s %d\n", input_buffer->requests[x].host, input_buffer->requests[x].retCode);
        }
        else if(input_buffer->requests[x].retCode == 200)
        {
            total200++;
            fprintf(stderr, "%s %d\n", input_buffer->requests[x].host, input_buffer->requests[x].retCode);
        }
    }*/



    int x;
    for(x = 0; x < N; x++)
    {
        total404 += stats404[x];
        total200 += stats200[x];

        fprintf(stderr, "404[%d] = %d\n", x, stats404[x]);
        fprintf(stderr, "\t\t\t200[%d] = %d\n", x, stats200[x]);
    }

    printf("CPU total 404: %d\n", cpuTotal404);
    printf("CPU total 200: %d\n", cpuTotal200);
    printf("Total 404s: %d\n", total404);
    printf("Total 200s: %d\n", total200);

    // cuda cleanup
    cudaFree(dev_buffer);
    cudaFree(dev_results404);
    cudaFree(dev_stats404);
    cudaFree(dev_results200);
    cudaFree(dev_stats200);
    cudaFree(results404);
    cudaFree(stats404);
    cudaFree(results200);
    cudaFree(stats200);

    cudaFree(input_buffer);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaStreamDestroy(stream0);
    cudaStreamDestroy(stream1);
    //cudaStreamDestroy(stream2);

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