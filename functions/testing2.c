#include <stdio.h>
#include <stdlib.h>

int main()
{
    sayHi("Erick", 24);
    return 0;
}

void sayHi(char name[], int age)
{
    printf("hello %s, you are %d\n", name, age);
}