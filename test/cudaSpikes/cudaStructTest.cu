/*
    cudaStructTest
    testing/optimizing how to access/manipulate/return
    structures in cuda.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "book.h"

#define N 30
#define TRUE 1
#define FALSE 0
#define MAX_BLOCKS 65000
/*#define BLOCKS 2
#define THREADS 5*/

int cuda_setup(int computeCapability);

typedef struct{
    int id;
    int age;
    int height;
} Person;


// Declare the Cuda kernels and any Cuda functions
__global__ void analyze_id(Person *people, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {
        Person person = people[id];

        if(person.id < 6)
        {
            statResults[id] = 1;
        }
        else
        {
            statResults[id] = 0;
        }
    }
    
}

__global__ void analyze_age(Person *people, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {
        Person person = people[id];

        if(person.age != 28)
        {
            statResults[id] = 1;
        }
        else
        {
            statResults[id] = 0;
        }
    }

}

__global__ void analyze_height(Person *people, int *statResults)
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if(id < N)
    {
        Person person = people[id];

        if(person.height != 6)
        {
            statResults[id] = 1;
        }
        else
        {
            statResults[id] = 0;
        }
    }
    
}


int main(void)
{
    int threads = N;
    int blocks = threads/2; // to be more conservative w/blocks: (N + (threads - 1)) / threads
    
    if(blocks > MAX_BLOCKS)
    {
        printf("Data set is too large\nSet cannot exceed %d elements\n", MAX_BLOCKS);
        exit(1);
    }

    cuda_setup(2);

    // CPU variables
    Person *people;
    int idStats[N];
    int ageStats[N];
    int heightStats[N];

    // GPU variables
    Person *dev_people;
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
    HANDLE_ERROR( cudaHostAlloc( (void **) &people, N * sizeof(Person), cudaHostAllocWriteCombined | cudaHostAllocMapped));

    // allocating GPU memory (GPU only memory)
    HANDLE_ERROR(cudaMalloc( (void **) &dev_idStats, N * sizeof(int) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_ageStats, N * sizeof(int) ));
    HANDLE_ERROR(cudaMalloc( (void **) &dev_heightStats, N * sizeof(int) ));

    /* FILL BUFFER WITH DATA */

    // fill people with some...
    Person person1  = {1, 25, 6};
    Person person2  = {2, 25, 5};
    Person person3  = {3, 25, 6};
    Person person4  = {4, 26, 5};
    Person person5  = {5, 26, 6};
    Person person6  = {6, 26, 5};
    Person person7  = {7, 27, 6};
    Person person8  = {8, 27, 5};
    Person person9  = {9, 27, 6};
    Person person10 = {10, 28, 5};

    Person person11  = {11, 25, 6};
    Person person12  = {12, 25, 5};
    Person person13  = {13, 25, 6};
    Person person14  = {14, 26, 5};
    Person person15  = {15, 26, 6};
    Person person16  = {16, 26, 5};
    Person person17  = {17, 27, 6};
    Person person18  = {18, 27, 5};
    Person person19  = {19, 27, 6};
    Person person20  = {20, 28, 5};

    Person person21  = {21, 25, 6};
    Person person22  = {22, 25, 5};
    Person person23  = {23, 25, 6};
    Person person24  = {24, 26, 5};
    Person person25  = {25, 26, 6};
    Person person26  = {26, 26, 5};
    Person person27  = {27, 27, 6};
    Person person28  = {28, 27, 5};
    Person person29  = {29, 27, 6};
    Person person30  = {30, 28, 5};

    people[0] = person1;
    people[1] = person2;
    people[2] = person3;
    people[3] = person4;
    people[4] = person5;
    people[5] = person6;
    people[6] = person7;
    people[7] = person8;
    people[8] = person9;
    people[9] = person10;

    people[10] = person11;
    people[11] = person12;
    people[12] = person13;
    people[13] = person14;
    people[14] = person15;
    people[15] = person16;
    people[16] = person17;
    people[17] = person18;
    people[18] = person19;
    people[19] = person20;

    people[20] = person21;
    people[21] = person22;
    people[22] = person23;
    people[23] = person24;
    people[24] = person25;
    people[25] = person26;
    people[26] = person27;
    people[27] = person28;
    people[28] = person29;
    people[29] = person30;

    // gives a pointer to the GPU to reference the zero-copy memory
    HANDLE_ERROR(cudaHostGetDevicePointer(&dev_people, people, 0));

    // calls to Cuda kernels
    analyze_id<<<blocks, threads>>>(dev_people, dev_idStats);
    analyze_age<<<blocks, threads>>>(dev_people, dev_ageStats);
    analyze_height<<<blocks, threads>>>(dev_people, dev_heightStats);

    // Get the results from the GPU
    HANDLE_ERROR(cudaMemcpy(idStats, dev_idStats, N * sizeof(int), cudaMemcpyDeviceToHost));
    HANDLE_ERROR(cudaMemcpy(ageStats, dev_ageStats, N * sizeof(int), cudaMemcpyDeviceToHost));
    HANDLE_ERROR(cudaMemcpy(heightStats, dev_heightStats, N * sizeof(int), cudaMemcpyDeviceToHost));

    // make sure everyone is done (only effective with multiple streams...)
    HANDLE_ERROR(cudaThreadSynchronize());

    //stop timing events
    HANDLE_ERROR(cudaEventRecord(stop, 0));
    HANDLE_ERROR(cudaEventSynchronize(stop));
    HANDLE_ERROR(cudaEventElapsedTime(&elapsedTime, start, stop));

    // cuda cleanup
    cudaFree(dev_people);
    cudaFree(dev_idStats);
    cudaFree(dev_ageStats);
    cudaFree(dev_heightStats);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

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

// checks system for cuda compatible devices;
// makes sure the cuda devices have the necessary compute capabilities;
// and picks the most suited device to compute on
// returns an int for status
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

