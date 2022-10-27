// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

//declaring variables of load
unsigned int word_count;
unsigned int hash_value;
// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO - if word exists return true, else return false
    //hash the word to obtain hashvalue
    hash_value = hash(word);
    //access linked list at that index of hastable
    node *cursor = table[hash_value];
    //traverse linked list looking for word
    while (cursor != 0)
    {
        //using strcasecmp, if found return true, else return false
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //adds ascii values of all char in word
    unsigned long sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //open dictionary file with fopen "r"
    FILE *file = fopen(dictionary, "r");
    //check if the file is null
    if (file == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }
    //read strings(words) one at a time
    //define word
    char word[LENGTH + 1];
    //use while loop with fscanf until reach EOF
    while (fscanf(file, "%s", word) != EOF)
    {
        //allocate memory for each node
        node *n = malloc(sizeof(node));
        //if malloc returns null, return false
        if (n == NULL)
        {
            return false;
        }
        //copy word to node with strcpy
        strcpy(n->word, word);
        //use hash to obtain hash value
        hash_value = hash(word);
        //insert node in hashtable at that location
        n->next = table[hash_value];
        table[hash_value] = n;
        word_count++;
    }
    //close file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO - free up allocated memory in CORRECT order
    //go through all buckets of hash table
    for (int i = 0; i < N; i++)
    {
        //point cursor to the start
        node *cursor = table[i];
        //while cursor is not null free memory
        while (cursor != NULL)
        {
            //create tmp variable
            node *tmp = cursor;
            //move cursor->next
            cursor = cursor->next;
            //free tmp
            free(tmp);
        }
        //if cursor is null return true
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
