#include <stdio.h>
#include <cs50.h>

//declare functions
int add_two_ints(int a, int b);

int main(void)
{
    //input 1
    int x = get_int("x: ");

    //input 2
    int y = get_int("y: ");

    //do the math
    int z = add_two_ints(x, y); // or int z = x + y;

    //give result
    printf("Result: %i\n", z);
}

//heres the math, or do directly on int z = (line 16)
int add_two_ints(int x, int y)
{
    int sum;
    sum = x + y;
    return sum;
}