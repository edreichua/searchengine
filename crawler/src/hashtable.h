/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a hashtable of urls.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

typedef struct HashTableNode {
    char *url;                               // url previously seen
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Public Variables
extern HashTable URLsVisited;

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/*
 * HashTableInsert - function to insert url in hash table
 * @url: url to insert
 * 
 * Returns 1 if URL is unique and also insert the url in hash table
 * returns 0 otherwise
 *
 */
int HashTableInsert(char* url);

/*
 * InitialiseHashTable - function to initialise hash table
 * @hashTable: HashTable to be initialised
 * 
 * Returns a HashTable with NULL nodes
 *
 */
void InitialiseHashTable(HashTable hashTable);

/*
 * CleanUpHashNode - function to free the HashNode
 * @hashnode: Pointer to HashTableNode to be freed
 * 
 *
 */
void CleanUpHashNode(HashTableNode* hashnode);

/*
 * CleanUpHash - function to free hash table
 * @hashtable: HashTable to be freed
 * 
 *
 */
void CleanUpHash(HashTable hashtable);

#endif // HASHTABLE_H
