#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{

    //check one string

    if (argc != 2)
    {
        printf("Usage: ./caesar key1\n");
        return 1;
    }

    //check digit

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key2\n");
            return 1;
        }
    }

    //convert

    int k = atoi(argv[1]);
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    //enciphering
    for (int j = 0; j < strlen(plaintext); j++)
    {
        //if is upper
        if (isupper(plaintext[j]))
        {
            printf("%c", (((plaintext[j] - 65) + k) % 26) + 65);
        }

        //if is lower
        else if (islower(plaintext[j]))
        {
            printf("%c", (((plaintext[j] - 97) + k) % 26) + 97);
        }

        //if its special char
        else
        {
            printf("%c", plaintext[j]);
        }

    }
    printf("\n");
}