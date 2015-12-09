/* ========================================================================== */
/* File: list.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Chua Zheng Fu Edrei
 * Date: 29 July 2015
 *
 */
/* ========================================================================== */

// ---------------- Open Issues
#include<inttypes.h>
// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "list.h"                      		 // hashtable functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types
List* WebPageList;

// ---------------- Private variables

// ---------------- Private prototypes
ListNode* InsertNode(ListNode* tail, ListNode* newentry);
ListNode* RemoveNode(ListNode* head);
void CleanUpPage(WebPage* page);
void CleanUpListNode(ListNode* listnode);
void CleanUpList(List* list);

// ---------------- Public functions

/*
 * InsertNode - function to insert node into tail of List
 * @tail: tail of the List
 * @newentry: ListNode to be added
 *
 * Returns the ListNode of the new tail 
 *
 */
ListNode* InsertNode(ListNode* tail, ListNode* newentry){
	tail->next = newentry;
	newentry->prev = tail;
	tail = newentry;
	return(tail);
}

/*
 * RemoveNode - function to remove node from head of List
 * @head: head of the List
 *
 * Returns the ListNode of the new head 
 *
 */
ListNode* RemoveNode(ListNode* head){
	if(head->next!=NULL){
		head = head->next;
		CleanUpListNode(head->prev);
		head->prev = NULL;
		return (head);
	}else{
		CleanUpListNode(head);
		return(NULL);
	}
}

/*
 * CleanUpPage - function to free WebPage
 * @page: WebPage to be freed
 * 
 *
 */
void CleanUpPage(WebPage* page){
	if(page!=NULL){
		free(page->url);
		page-> url = NULL;
		free(page->html);
		page->html = NULL;
		free(page);
		page = NULL;
	}
}

/*
 * CleanUpListNode - function to free ListNode
 * @listnode: ListNode to be freed
 * 
 *
 */
void CleanUpListNode(ListNode* listnode){
	CleanUpPage(listnode->page);
	free(listnode);
	listnode = NULL;
}

/*
 * CleanUpList - function to free the List
 * @list: List to be freed
 * 
 *
 */
void CleanUpList(List* list){
	ListNode* newhead = list->head;
	while(newhead != NULL){
		newhead = RemoveNode(newhead);
	}
	free(list);
	list = NULL;
}