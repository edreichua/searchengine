/* ========================================================================== */
/* File: list.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions for a doubly-linked list of WebPages.
 *
 */
/* ========================================================================== */
#ifndef LIST_H
#define LIST_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "common.h"                          // common functionality
#include "hashtable.h"

// ---------------- Constants

// ---------------- Structures/Types

typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list.
  int doc_id;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;



// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * ListInsert: function to insert DocumentNode into the list
 * @doc: the document id
 * @dnode: the pointer to the head of the first DocumentNode in the WordNode
 * 
 * Returns 1 if doc_id is unique, returns 0 for repeated cases
 * Special consideration: dnode should be declared and memory allocated before passing into the function
 *
 * Pseudocode:	1. Initialise node if the word is found for the first time
 *				2. If not, search through the single linked list
 *						a. if a document node with the same id is found, increment the freq
 *						b. if no nodes with the same id can be found, create a new node and initialise it
 *
 */
int ListInsert(int doc, DocumentNode** dnode);


/*
 * CleanUpList - function to free the Single linked list
 * @head: the head of the single linked list
 * Special Consideration: It is the caller responsibility to ensure that the head of the 
 *		linked list, and not an intermediate node, is passed into the function to prevent 
 *		memory leak
 * 
 * Pseudocode: Save the pointer to the next node, and free the previous node. Keep doing
 *				this until the next node is a NULL pointer
 *
 */
void CleanUpList(DocumentNode* head);

#endif // LIST_H
