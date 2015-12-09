/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Chua Zheng Fu Edrei
 * Date: 29 July 2015
 *
 * You should include in this file your functionality for the hashtable as
 * described in the assignment and lecture.
 */
/* ========================================================================== */

// ---------------- Open Issues
#include<inttypes.h>
// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "hashtable.h"                       // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types
HashTable URLsVisited;

// ---------------- Private variables

// ---------------- Private prototypes
unsigned long JenkinsHash(const char *str, unsigned long mod);
int HashTableInsert(char* url);
void InitialiseHashTable(HashTable hashTable);
void CleanUpHashNode(HashTableNode* hashnode);
void CleanUpHash(HashTable hashtable);

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
 * HashTableInsert - function to insert url in hash table
 * @url: url to insert
 * 
 * Returns 1 if URL is unique and also insert the url in hash table
 * returns 0 otherwise
 *
 */
int HashTableInsert(char* url){

	unsigned long hashnum = JenkinsHash(url, MAX_HASH_SLOT);
	HashTableNode **node = &URLsVisited.table[hashnum];
	
	if(*node==NULL){
		*node = (HashTableNode*)calloc(1,sizeof(HashTableNode));
		(*node)->url = (char*)calloc(strlen(url)+1,sizeof(char));
		strcpy((*node)->url, url);
		(*node)->next = NULL;
		return 1;
	}else{
		while(*node!=NULL){
			if(!strcmp((*node)->url,url)) return 0;
			node = &((*node)->next);
		}
		*node = (HashTableNode*)calloc(1,sizeof(HashTableNode));
		(*node)->url = (char*)calloc(strlen(url)+1,sizeof(char));
		strcpy((*node)->url, url);
		(*node)->next = NULL;
		return 1;
	}
}

/*
 * InitialiseHashTable - function to initialise hash table
 * @hashTable: HashTable to be initialised
 * 
 * Returns a HashTable with NULL nodes
 *
 */
void InitialiseHashTable(HashTable hashTable){
	for(int i=0; i<MAX_HASH_SLOT; i++){
		hashTable.table[i] = NULL;
	}
}

/*
 * CleanUpHashNode - function to free the HashNode
 * @hashnode: Pointer to HashTableNode to be freed
 * 
 *
 */
void CleanUpHashNode(HashTableNode* hashnode){
	HashTableNode* ptrB = NULL;
	for(HashTableNode* ptrA = hashnode; ptrA != NULL; ptrA = ptrB){
			ptrB = ptrA->next;
			free(ptrA->url);
			ptrA->url = NULL;
			free(ptrA);
			ptrA = NULL;
	}
}

/*
 * CleanUpHash - function to free hash table
 * @hashtable: HashTable to be freed
 * 
 *
 */
void CleanUpHash(HashTable hashtable){
	for(int i=0; i<MAX_HASH_SLOT; i++){
		if(hashtable.table[i]!=NULL){
			CleanUpHashNode(hashtable.table[i]);
		}
	}	
}