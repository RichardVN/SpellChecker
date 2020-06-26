/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Richard Nguyen
 * Date: 3/10/2020
 */
#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                void * tmp = realloc(word, maxLength);
                if (NULL == tmp)
                {
                    free(tmp);
                }
                else
                {
                    word = tmp;
                }
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        // free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
 //Note: dictionary.txt is passed as "file" parameter

    // set current word to first word in file
    char * currentWord = nextWord(file);



   while(currentWord != NULL)
    {
        // put word into map. Key = word, value = Levenshtein distance
        hashMapPut(map, currentWord, 0);
        // hashMapPrint(map);        
        // Test: print dictionary
        // printf("word: %s count: %d\n" );
        

        //get next word
        currentWord = nextWord(file);
    }
}

/**
 * Calculates the Levenshtein distance between two strings. The integer 
 * result is the minimum number of single-character edits to change
 * string s into string t. 
 * @param s   first string
 * @param st   second string
 * @return
 * References : https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C
**/
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    // create new empty hash map with capacity 1000
    HashMap* map = hashMapNew(1000);

    // declare pointer of FILE type and assign to dictionary.txt
    FILE* file = fopen("dictionary.txt", "r"); // "r" designates to read from it

    clock_t timer = clock();
    // populate hash map from dictionary file
    loadDictionary(file, map);
    // calculate time to load dictionary
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds \n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);


    char inputBuffer[256];
    int quit = 0;
   
    //create array of pointers to HashLink ptrs to hold suggested words
        // usage ex.: suggested[1] = current
    HashLink *suggested[5];
    for (int i = 0; i < 5; ++i)
    {
        suggested[i] = NULL;
    }

    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        // Implement the spell checker code here.. 
        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
        // if buffer matches key in map 
        else if (hashMapContainsKey(map,inputBuffer))
        {
            printf("The inputted word %s is spelled correctly! \n", inputBuffer);
        }
        // no matching key
        else
        {
            // print suggested words
            printf("The inputted word %s is spelled incorrectly! \nCalculating suggestions...\n", inputBuffer);
              // iterate through HashMap and update values with Levenshtein Distance for current word
              for (int i = 0; i < hashMapCapacity(map); ++i)
                  {
                      // create ptr to HashLink to iterate through table
                      HashLink * current = map->table[i];

                      if (current != NULL) // if current bucket is not empty
                      {
                          // iterate through bucket bucket
                            while(current != NULL)
                            {
                                // calculate levenshtein for current key vs inputBuffer
                                // set high l distance for words of significantly different lengths
                                int levenDistance = 15;
                                // update levenshtein value for words of close length
                                if (strlen(current->key) <= strlen(inputBuffer)+1 || strlen(current->key) >= strlen(inputBuffer)-1)
                                {
                                    levenDistance = levenshtein(inputBuffer, current->key);
                                }
                                
                                
                                // update value (initialized as 0), to the calculated L distance
                                hashMapPut(map, current->key, levenDistance);
                                // print for testing
                                // printf("\nKey: %s, LevenDistance: %d", current->key, current->value );
                               
                                // compare L distance to see if lower than any L value in suggested array
                                for (int j = 0; j < 5; ++j)
                                {
                                    if (suggested[j] == NULL || levenDistance < suggested[j]->value)
                                    {
                                        suggested[j] = current; 
                                        break;

                                    }
                                    
                                }
                                // next link, eventually NULL
                                current = current->next;
                            }
                      }
                  }
                                
                printf("Did you mean one of these words? \n");
                for (int i = 0; i < 5; ++i)
                {
                    printf("%s ", suggested[i]->key);
                }
                printf("\n\n");
        }
    
    }

    hashMapDelete(map);

    return 0;
}


