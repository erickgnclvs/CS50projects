#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //prompt user to agree
    char c = get_char("do you agree? ");

    //check whether user agree
    if (c == 'y' || c == 'Y')
    {
        printf("agreed.\n");
    }
    else if (c == 'n' || c == 'N')
    {
        printf("dont agree.\n");
    }
}