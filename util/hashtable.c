/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * Author: Chua Zheng Fu Edrei
 * Date: 9 August 2015
 *
 */
/* ========================================================================== */


// ---------------- System includes e.g., <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality
#include "list.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
unsigned long JenkinsHash(const char *str, unsigned long mod);
int HashTableInsert(char* word, int doc, HashTable* index);
void InitialiseHashTable(HashTable* hashTable);
void CleanUpHash(HashTable* hashtable);

// ---------------- Public functions
unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

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
int HashTableInsert(char* word, int doc, HashTable* index){
	//Obtain the node in the hashtable by looking up the hash number
	unsigned long hashnum = JenkinsHash(word, MAX_HASH_SLOT);
	WordNode **node = &(index->table[hashnum]);
	
	// If the node is NULL, word is found for the first time
	if(*node==NULL){		
		*node = (WordNode*)calloc(1,sizeof(WordNode));
		(*node)->word = (char*)calloc(strlen(word)+1,sizeof(char));
		strcpy((*node)->word, word);
		(*node)->docfreq = 1;
		(*node)->page = calloc(1, sizeof(DocumentNode));
		ListInsert(doc,&((*node)->page));
		(*node)->next = NULL;
		return 1;
	}else{ 
		while(*node!=NULL){
			if((*node)->word!= NULL && !strcmp((*node)->word,word)){
				if(ListInsert(doc,&((*node)->page))==1){ 	// new document, increment docfreq by 1
					(*node)->docfreq = (*node)->docfreq + 1;
				}
				return 0;
			}
			node = &((*node)->next);
		}
		*node = (WordNode*)calloc(1,sizeof(WordNode));
		(*node)->word = (char*)calloc(strlen(word)+1,sizeof(char));
		strcpy((*node)->word, word);
		(*node)->docfreq = 1;
		(*node)->page = calloc(1, sizeof(DocumentNode));
		ListInsert(doc,&((*node)->page));
		(*node)->next = NULL;
		return 2;
	}
}


/*
 * InitialiseHashTable - function to initialise hash table
 * @hashTable: HashTable to be initialised
 * 
 * Returns a HashTable with NULL nodes
 *
 */
void InitialiseHashTable(HashTable* hashTable){
	for(int i=0; i<MAX_HASH_SLOT; i++){
		hashTable->table[i] = NULL;
	}
}


/*
 * CleanUpHash - function to free the Hash table
 * @hashtable: pointer to the hashtable
 * 
 * Pseudocode: for all rows in the hashtable
 *					Save the pointer to the next WordNode, and free the previous node. 
 *					Keep doing this until the next node is a NULL pointer
 *				
 */
void CleanUpHash(HashTable* hashtable){
	for(int i=0; i<MAX_HASH_SLOT; i++){
		WordNode* ptrB = NULL;
		for(WordNode* ptrA = hashtable->table[i]; ptrA != NULL; ptrA = ptrB){
			ptrB = ptrA->next;
			free(ptrA->word);
			CleanUpList(ptrA->page);
			free(ptrA);
		}
	}
}
