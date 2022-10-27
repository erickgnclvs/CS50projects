#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(void)
{
    //get fav color
    string color = get_string("whats you fav color? ");

    //print fav color
    printf("your fav color is %s.\n", color);
}