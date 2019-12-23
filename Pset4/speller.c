// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 10000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hash function allow us to get the index  and  it is case insensitive
unsigned int hash(const char *word)
{
    unsigned long  sum = 0;
    for (int j = 0; word[j] != '\0'; j++)
    {
        sum = 31*sum + tolower(word[j]) ;
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {

        // Create a new node
        node *new_node = malloc(sizeof(node));
        // Check if couldn't allocate memory for new_node
        if (new_node == NULL)
        {
            // Free all created nodes if any
            unload();
            return false;
        }
        // Copy the word from our dictionary to new_node
        strcpy(new_node -> word, word);

        // Get the index where to put our new_node in the Hashtable
        unsigned int index = hash(word);

        // Add the new_node to our hashtable a location hastable[index]
        if (hashtable[index] == NULL)
        {
            new_node -> next = NULL;
            hashtable[index] = new_node;
        }
        else
        {
            new_node -> next = hashtable[index];
            hashtable[index] = new_node ;
        }

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Variable to hold number of words
    unsigned int dict_words = 0;
                              for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];

        // Check if our linked list in hastable[i] is not empty
        if (cursor == NULL)
        {
            // Do nothing because our linked lsit  is empty
        }
        else
        {
            // Count number of nodes in linked list at hashtable[i]
            while (cursor != NULL)
            {
                dict_words ++;
                cursor = cursor->next;
            }
        }
    }
    return dict_words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Find the location of the  linked list in hashtable using hash function
    unsigned int index = hash(word);
    node *cursor = hashtable[index];

    // Boollean variable to remind us if the word is in our hashtable(linked list at location hashtable[index])
    bool  indicator = false;

    // Iterate through the  linked list at hahshtable[index]
    while (cursor != NULL)
    {
        indicator  = false ;
        // Make the iteration through each char of word easier
        int i = 0 ;

        // Variable to make our char comparaison easier
        char *node_word = cursor->word;
        const char *word_copy = word;

        // Compare char (case-insensivity)
        while (word_copy[i] != '\0' || word_copy[i] != '\0' )
        {
            if (tolower(word_copy[i]) == (node_word[i]) && isalpha(node_word[i]) && isalpha(word_copy[i]))
            {
                // Do nothing because they are equals and alpha
            }
            else if (!isalpha(word_copy[i]) && !isalpha(node_word[i]) && node_word[i] == word_copy[i])
            {
                // Do nothing because they are equals and not alpha
            }
            else
            {
                // Break because the two char are not the same (reduce runing time)
                indicator = true ;
                break ;
            }

            i++;
        }
        // Retun true if the words are the same
        if (indicator == false && strlen(word_copy) == strlen(node_word) )
        {
            return true;
        }



    cursor = cursor -> next;
    }

    // Return false if no equal word was find
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through each linked list in our hastable at hastable[i]
    for(int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];
        // If hastable[i] is empty
        if (cursor == NULL)
        {
            // Do nothing (reduce running time)
        }
        else
        {
            while (cursor !=0)
            {
                node *temporary = cursor ;
                cursor = cursor->next;
                free(temporary);
            }
        }
    }
    return true;
}
