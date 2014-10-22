/*
    cudaStructTest
    testing/optimizing how to access/manipulate/return
    structures in cuda
*/

#include <stdio.h>
#include <unistd.h>

#define N 10

typedef struct{
    int id;
    int age;
    int height;
} Person;

Person * populate_array();

__global__ void analyze_id()
{

}

__global__ void analyze_age()
{
    
}

__global__ void analyze_height()
{
    
}


int main(void)
{
    Person * people;
    people = populate_array();

    int x;
    for(x = 0; x < 1; x++)
    {
        printf("id = %d\n", people[x].id);
        printf("age = %d\n", people[x].age);
        printf("height = %d\n", people[x].height);
        printf("\n");
    }

    return 0;


}

Person * populate_array()
{
    Person people[10];
    //people = malloc(N * sizeof(Person));

    Person person1 = {1, 25, 6};
   /* Person *person2 = {2, 25, 6};
    Person *person3 = {3, 25, 6};
    Person *person4 = {4, 25, 6};
    Person *person5 = {5, 25, 6};
    Person *person6 = {6, 25, 6};
    Person *person7 = {7, 25, 6};
    Person *person8 = {8, 25, 6};
    Person *person9 = {9, 25, 6};
    Person *person10 = {10, 25, 6};*/

    people[0] = person1;
   /* people[1] = person2;
    people[2] = person3;
    people[3] = person4;
    people[4] = person5;
    people[5] = person6;
    people[6] = person7;
    people[7] = person8;
    people[8] = person9;
    people[9] = person10;*/

    return people;

}