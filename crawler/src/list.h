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

// ---------------- Constants

// ---------------- Structures/Types

typedef struct ListNode {
    WebPage *page;                           // the data for a given page
    struct ListNode *prev;                   // pointer to previous node
    struct ListNode *next;                   // pointer to next node
} ListNode;

typedef struct List {
    ListNode *head;                          // "beginning" of the list
    ListNode *tail;                          // "end" of the list
} List;

// ---------------- Public Variables
extern List* WebPageList;

// ---------------- Prototypes/Macros

/*
 * InsertNode - function to insert node into tail of List
 * @tail: tail of the List
 * @newentry: ListNode to be added
 *
 * Returns the ListNode of the new tail 
 *
 */
ListNode* InsertNode(ListNode* tail, ListNode* newentry);


/*
 * RemoveNode - function to remove node from head of List
 * @head: head of the List
 *
 * Returns the ListNode of the new head 
 *
 */
ListNode* RemoveNode(ListNode* head);


/*
 * CleanUpPage - function to free WebPage
 * @page: WebPage to be freed
 * 
 *
 */
void CleanUpPage(WebPage* page);


/*
 * CleanUpListNode - function to free ListNode
 * @listnode: ListNode to be freed
 * 
 *
 */
void CleanUpListNode(ListNode* listnode);


/*
 * CleanUpList - function to free the List
 * @list: List to be freed
 * 
 *
 */
void CleanUpList(List* list);

#endif // LIST_H
