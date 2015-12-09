/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: indexer
 *
 * This file contains the definitions for a hashtable for indexer
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality
#include "list.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types

typedef struct WordNode {
  struct WordNode *next;            		// pointer to the next word (for collisions)
  char *word;                      			// the word
  int docfreq;								// number of documents with the word
  struct DocumentNode *page;              	// pointer to the first element of the page list.
} WordNode;	

typedef struct HashTable {
    struct WordNode* table[MAX_HASH_SLOT];  // actual hashtable
} HashTable;

// ---------------- Public Variables

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
 * HashTableInsert - function to insert words and document id into the data structure
 * @word: word to insert into the inverted indes
 * @doc: document id of the associated word
 * @index: pointer to the hashtable
 *
 * Returns the hashtable index with the updates
 * function returns 1 if the word is found for the first time;
 *		2 if the word is found in the document with id doc for the first time;
 *		0 if the word has already been found in the same document previously
 *
 * Special Consideration: It is the user responsibility to pass the hashtable to be updated
 *							into the function 
 * 
 * Pseudocode: 1. Obtain the node in the hashtable by looking up the hash number
 *			   2. If the node is NULL, word is found for the first time
 *					a. Initialise word node
 *					b. Create DocumentNode
 *			   3. If node is not NULL
 *					a. search through the WordNodes
 *					b. if at any point the same word occur, increment the word freq and we are done
 *					c. if we reached the end of the list and the word is still not found, create
 *							a new WordNode and DocumentNode
 */
int HashTableInsert(char* word, int doc, HashTable* index);

/*
 * InitialiseHashTable - function to initialise hash table
 * @hashTable: HashTable to be initialised
 * 
 * Returns a HashTable with NULL nodes
 *
 */
void InitialiseHashTable(HashTable* hashTable);


/*
 * CleanUpHash - function to free the Hash table
 * @hashtable: pointer to the hashtable
 * 
 * Pseudocode: for all rows in the hashtable
 *					Save the pointer to the next WordNode, and free the previous node. 
 *					Keep doing this until the next node is a NULL pointer
 *				
 */
void CleanUpHash(HashTable* hashtable);

#endif // HASHTABLE_H
