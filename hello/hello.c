#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask name
    int answer = get_string("What's your name? ");
    //say hello, name
    printf("Hello, %s\n", answer);
}