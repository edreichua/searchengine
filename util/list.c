/* ========================================================================== */
/* File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * Author: Chua Zheng Fu Edrei
 * Date: 9 August 2015
 *
 * Single-linked list of DocumentNode revised from the double-linked list code in Crawler
 * Special Consideration: the head of the list is stored in a pointer in the WordNode
 *
 */
/* ========================================================================== */

// ---------------- Open Issues
#include<inttypes.h>
// ---------------- System includes e.g., <stdio.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "list.h"                      		 
#include "hashtable.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int ListInsert(int doc, DocumentNode** dnode);
void CleanUpList(DocumentNode* head);

// ---------------- Public functions

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
int ListInsert(int doc, DocumentNode** dnode){
	// Initialise node if the word is found for the first time
	if ((*dnode)->freq == 0){
		(*dnode)->doc_id = doc;
		(*dnode)->freq = 1;
		(*dnode)->next = NULL;
		return 1;
	}
	
	while(*dnode != NULL){
		if( (*dnode)->doc_id == doc){
			(*dnode)->freq = (*dnode)->freq + 1;			// increment freq by 1
			return 0;
		}
		dnode = &((*dnode)->next);
	}
	
	*dnode = calloc(1,sizeof(DocumentNode));
	(*dnode)->doc_id = doc;
	(*dnode)->freq = 1;
	(*dnode)->next = NULL;
	return 1;
}

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
 
void CleanUpList(DocumentNode* head){
	DocumentNode* d2 = NULL;
	for(DocumentNode* d1 = head; d1 != NULL; d1 = d2){
		d2 = d1->next;
		free(d1);
	}
}
