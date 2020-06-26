/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Richard Nguyen
 * Date: 3/10/2020
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    // step through each char in string
    for (int i = 0; key[i] != '\0'; i++)
    {
        // add value ascii value of char to r
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    // step through each char in string 
    for (int i = 0; key[i] != '\0'; i++)
    {
        // multiply char ascii value by (index + 1)
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    // initialize capacity to param, initialize size as 0
    map->capacity = capacity;
    map->size = 0;
    // allocate table to hold pointers to HashLinks
    map->table = malloc(sizeof(HashLink*) * capacity);
    // initialize each table index as empty
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    assert(map != 0); 

    // Step through each bucket of hash map
    for(int i = 0; i < map->capacity; i++)
    {
        // set current link node to the link list heads
        struct HashLink* current = map->table[i]; 

        // step through each node and delete node
        while(current != 0)
        {
            struct HashLink* temp = current; 
            current = current->next; 
            hashLinkDelete(temp); 
        }
        free(current); 
    }
    /*Free the table and set it's size to 0*/
    free(map->table); 
    map->size = 0; 
}



/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
   // printf("GET\n");
    // find index of correct bucket
        int hashIndex = HASH_FUNCTION(key) % map->capacity;
    // no negative index allowed, convert to positive
    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // current, ptr to HashLink. go through links in bucket to see if key exists
    struct HashLink *current = map->table[hashIndex];
    while(current != NULL)
    {
        if (strcmp(key, current->key) == 0) // if key strings are equal
        {
            // return pointer (address) to value of the link
            return &(current->value);
        }
        else// no matching key, traverse to next link in bucket
        {
            current = current->next;
        }
    }
   
    // no match to given key, return NULL ptr
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given 
 * capacity (double of the old capacity). After allocating the new table, 
 * all of the links need to rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
     // allocate new map with capacity
    struct HashMap *tempMap = hashMapNew(capacity);

    // Current is a link ptr to transverse each list
    struct HashLink *current;

    // step through each bucket/list
    for(int i = 0; i < hashMapCapacity(map); i++)
    {
        current = map->table[i];
        
        // copy links to tempMap
        //NOTE: hashMapPut rehashes all values according to tempMap capacity
        // current transverses through each bucket
        while(current != NULL){
            hashMapPut(tempMap, current->key, current->value);
            current = current->next;
        }
    }

    // free table and copy table from tempMap
    hashMapCleanUp(map); // set map->table to NULL
    map->table = tempMap->table;

    //copy attributes from tempMap
    map->size = tempMap->size;
    map->capacity = tempMap->capacity;
    
    // Free the temp map
    free(tempMap);
    tempMap = NULL;
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 * 
 * @param map
 * @param key
 * @param value
 * Refernce worksheet 38
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // printf("PUT \n");
    // FIXME: implement
    // compute hash value to find correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;
    // no negative index allowed, convert to positive
    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // current, ptr to HashLink. go through links in bucket to see if key exists
    struct HashLink *current = map->table[hashIndex];
    while(current != NULL)
    {
        if (strcmp(key, current->key) == 0) // if key strings are equal
        {
            current->value = value; // update value
            return; // skip traversing, exit function
        }
        else// no matching key, traverse to next link in bucket
        {
            current = current->next;
        }
    }

 
    // no link with given key. Create new link and add to bucket at front of list
    // create newLink whose next ptr points to front of list map->[hashIndex]
    struct HashLink *newLink = hashLinkNew(key, value, map->table[hashIndex]); // note: 3rd param is next ptr of newLink
    //reassign ptr to front of list to newLink
    map->table[hashIndex] = newLink;
    // increment size of hash map
    map->size++;

    //resize if necessary if load factor too big
    if (hashMapTableLoad(map) >= MAX_TABLE_LOAD) // MAX_TABLE_LOAD defined as 1
    {
        resizeTable(map, (map->capacity) * 2);
    }


}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
// reference wk 38
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement'
    // test print
    // printf("hashmapremove\n");
    // compute hash value to find correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;
    // no negative index allowed, convert to positive
    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // current iterator
    struct HashLink *current = map->table[hashIndex];
    // previous position
    struct HashLink *prev = 0;

    while(current != NULL)
    {
        if (strcmp(key, current->key) == 0) // Value found
        {
            if (prev == 0) // value is at first entry
            {
                map->table[hashIndex] = current->next;
            }
            else // value found in middle of list, must link prev and next
            {
                prev->next = current->next;
            }
        
            // free link, decrement count
            free(current);
            map->size--;
            return;
        }
        else// value not found yet, prev is assigned to current
        {
            prev = current;
            current = current->next;
        }
    }
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // printf("CONTAINS\n");
    // FIXME: implement
    // compute hash value to find correct bucket
    int hashIndex = HASH_FUNCTION(key) % map->capacity;
    // no negative index allowed, convert to positive
    if (hashIndex < 0)
    {
        hashIndex += map->capacity;
    }

    // create current node to search through bucket
    struct HashLink * current;
    // current at head of bucket list
    current = map->table[hashIndex];

    // transverse through bucket
    while(current != NULL)
    {
        if (strcmp(key, current->key) == 0) // if key strings are equal
        {
            return 1;// return 1 for true, exit function
        }
        else// no matching key, traverse to next link in bucket
        {
            current = current->next;
        }
    }

    return 0;// key not found, return false
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // return number of links
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // return capacity / number of buckets
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
   // printf("HAS EMPTY\n");
    // empty bucket counter
    int emptyBuckets = 0;
    // iterate through each bucket
    for (int i = 0; i < hashMapCapacity(map); ++i)
    {
        // if bucket is NULL, increment counter
        if (map->table[i] == NULL)
        {
            emptyBuckets++;
        }
    }
    return emptyBuckets;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    float load = (float)(hashMapSize(map)/hashMapCapacity(map));//cast as float
    return load;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
  if (hashMapSize(map)==0)
  {
      printf("Hash map is empty. \n");
  }

  for (int i = 0; i < hashMapCapacity(map); ++i)
  {
      // create ptr to HashLink to transverse bucket
      HashLink * current = map->table[i];

      if (current != NULL) // if current bucket is not empty
      {
          // transverse bucket
            // print current bucket
            printf("\n\nBUCKET NUMBER: %d",i );
            while(current != NULL)
            {
                printf("\nKey: %s, Value: %d", current->key, current->value );
                // next link, eventually NULL
                current = current->next;
            }

      }

  }
  printf("\n");

   
}
