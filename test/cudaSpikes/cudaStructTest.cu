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

Person * populate_array(Person *people);

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
    Person *people = malloc(N * sizeof(Person));
    people = populate_array(people);

    return 0;


}

Person * populate_array(Person *people)
{
    Person person1;
    person1.id = 1;
    person1.age = 25;
    person1.height = 6;

    Person person2;
    person2.id = 2;
    person2.age = 21;
    person2.height = 5;

    Person person3;
    person3.id = 3;
    person3.age = 26;
    person3.height = 6;

    Person person4;
    person4.id = 4;
    person4.age = 23;
    person4.height = 5;

    Person person5;
    person5.id = 5;
    person5.age = 25;
    person5.height = 7;

    Person person6;
    person6.id = 6;
    person6.age = 27;
    person6.height = 5;

    Person person7;
    person7.id = 7;
    person7.age = 29;
    person7.height = 6;

    Person person8;
    person8.id = 8;
    person8.age = 24;
    person8.height = 6;

    Person person9;
    person9.id = 9;
    person9.age = 27;
    person9.height = 6;

    Person person10;
    person10.id = 10;
    person10.age = 30;
    person10.height = 2;

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

    return people;

}