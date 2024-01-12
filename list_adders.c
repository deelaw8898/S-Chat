#include <stdlib.h>
#include <stdio.h>
#include <list.h>
#include <string.h>

#define MIN_NODE_COUNT 500
#define MIN_LIST_COUNT 50

/*
 * These arrays will allocate the memory we need at the start of our program
 */
LIST listMemory[MIN_LIST_COUNT];
NODE nodeMemory[MIN_NODE_COUNT];

/*
 * Will have a flag that will be set once our first listCreate is called.
 * When we call list create the first time, we will link all of our nodes
 * to eachother
 */
int listInitialized = 0;
NODE* freeNodes;
LIST* freeLists;

/*
 * Creates an empty list structure.
  @return A reference to our newly created empty list
 */
LIST* ListCreate() {
	int i = 0;
	int iterationLength;
	int subIterationLength;
	LIST* nextFree;
	LIST* returnList;
	
	if (MIN_LIST_COUNT > MIN_NODE_COUNT) {
		iterationLength = MIN_LIST_COUNT;
		subIterationLength = MIN_NODE_COUNT;

	    if (!listInitialized) {
		    nodeMemory[0].next = &nodeMemory[1];
		    listMemory[0].next = &listMemory[1];
		    for (i = 1; i < iterationLength - 1; i++) {
			    if (i < subIterationLength - 1) {
				    nodeMemory[i].prev = &nodeMemory[i-1];
				    nodeMemory[i].next = &nodeMemory[i+1];
			    }
			    listMemory[i].prev = &listMemory[i-1];
			    listMemory[i].next = &listMemory[i+1];
		    }
		    nodeMemory[MIN_NODE_COUNT-1].prev = &nodeMemory[MIN_NODE_COUNT-2];
		    listMemory[MIN_LIST_COUNT-1].prev = &listMemory[MIN_LIST_COUNT-2];
		    freeNodes = &nodeMemory[MIN_NODE_COUNT-1];
		    freeLists = &listMemory[MIN_LIST_COUNT-1];

		    listInitialized = 1;
	    }
	}
	else {
		iterationLength = MIN_NODE_COUNT;
		subIterationLength = MIN_LIST_COUNT;
	    
        if (!listInitialized) {
		    nodeMemory[0].next = &nodeMemory[1];
		    listMemory[0].next = &listMemory[1];
		    for (i = 1; i < iterationLength - 1; i++) {
			    if (i < subIterationLength - 1) {
				    listMemory[i].prev = &listMemory[i-1];
				    listMemory[i].next = &listMemory[i+1];
			    }
			    nodeMemory[i].prev = &nodeMemory[i-1];
			    nodeMemory[i].next = &nodeMemory[i+1];
		    }
		    nodeMemory[MIN_NODE_COUNT-1].prev = &nodeMemory[MIN_NODE_COUNT-2];
		    listMemory[MIN_LIST_COUNT-1].prev = &listMemory[MIN_LIST_COUNT-2];
		    freeNodes = &nodeMemory[MIN_NODE_COUNT-1];
		    freeLists = &listMemory[MIN_LIST_COUNT-1];

		    listInitialized = 1;
	    }
	}

	if (!listInitialized) {
		nodeMemory[0].next = &nodeMemory[1];
		listMemory[0].next = &listMemory[1];
		for (i = 1; i < iterationLength - 1; i++) {
			if (i < subIterationLength - 1) {
				listMemory[i].prev = &listMemory[i-1];
				listMemory[i].next = &listMemory[i+1];
			}
			nodeMemory[i].prev = &nodeMemory[i-1];
			nodeMemory[i].next = &nodeMemory[i+1];
		}
		nodeMemory[MIN_NODE_COUNT-1].prev = &nodeMemory[MIN_NODE_COUNT-2];
		listMemory[MIN_LIST_COUNT-1].prev = &listMemory[MIN_LIST_COUNT-2];
		freeNodes = &nodeMemory[MIN_NODE_COUNT-1];
		freeLists = &listMemory[MIN_LIST_COUNT-1];

		listInitialized = 1;
	}
	else if (freeLists == NULL) {
		char* msg = "Error, no remaining list memory\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	returnList = freeLists;
	
	nextFree = (LIST*)freeLists->prev;
	
	freeLists->next = NULL;
	freeLists->prev = NULL;
	
	freeLists = nextFree;
	
	return returnList;
}

/*
 * Adds an item to the list after the item the cursor is pointing at
 * @param list: The list we are adding to
 * @param item: The item we are going to add to our list
 * @return 0 if the item was successfully added, -1 if adding the item
 * failed
 */
int ListAdd(LIST* list, void* item) {
	NODE* nextFree;
	if (item == NULL) {
		return ERROR_EXIT;
	}
	else if (list == NULL) {
		return ERROR_EXIT;
	}
	else if (freeNodes == NULL) {
		return ERROR_EXIT;
	}
	else if (list->count == 0) {
		nextFree = (NODE*)freeNodes->prev;
	
		list->head = freeNodes;
		list->tail = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes->item = item;	
		freeNodes->next = NULL;
		freeNodes->prev = NULL;

		freeNodes = nextFree;
		
		return SUCCESS_EXIT;
	}
	else {
		NODE* currentNode;
		NODE* currentNextNode;
		
		nextFree = (NODE*)freeNodes->prev;
		
		currentNode = list->curr;
		currentNextNode = (NODE*) currentNode->next;
		
		if (currentNextNode == NULL) {
			list->tail = freeNodes;
		}
		else {
			currentNextNode->prev = freeNodes;	
		}
		
		currentNode->next = freeNodes;
		
		freeNodes->item = item;
		freeNodes->prev = currentNode;
		freeNodes->next = currentNextNode;
		
		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes = nextFree;
		
		return SUCCESS_EXIT;
	}
}

/*
 * Adds an item to the list before the item the cursor is pointing at
 * @param list: The list we are adding to
 * @param item: The item we are going to add to our list
 * @return 0 if the item was succesfully added, -1 if adding the item
 * failed
 */
int ListInsert(LIST* list, void* item) {
	NODE* nextFree;
	if (item == NULL) {
		return ERROR_EXIT;
	}
	else if (list == NULL) {
		return ERROR_EXIT;
	}
	else if (freeNodes == NULL) {
		return ERROR_EXIT;
	}
	else if (list->count == 0) {
		nextFree = (NODE*)freeNodes->prev;

		list->head = freeNodes;
		list->tail = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes->item = item;
		freeNodes->next = NULL;
		freeNodes->prev = NULL;

		freeNodes = nextFree;

		return SUCCESS_EXIT;
	}
	else {
		NODE* currentNode;
		NODE* currentPrevNode;
		
		nextFree = (NODE*)freeNodes->prev;
		
		currentNode = list->curr;
		currentPrevNode = (NODE*) currentNode->prev;
		
		if (currentPrevNode == NULL) {
			list->head = freeNodes;
		}
		else {
			currentPrevNode->next = freeNodes;
		}
		
		currentNode->prev = freeNodes;
		
		freeNodes->item = item;
		freeNodes->prev = currentPrevNode;
		freeNodes->next = currentNode;

		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes = nextFree;

		return SUCCESS_EXIT;
	}
}

/*
 * Adds an item to the end of the list
 * @param list: The list we are adding to
 * @param item: The item we are going to add to our list
 * @return 0 if the item was successfully added, -1 if adding the item
 * failed
 */
int ListAppend(LIST* list, void* item) {
	NODE* nextFree;
	if (item == NULL) {
		char* msg = "Error, cannot append null item to list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		return ERROR_EXIT;
	}
	else if (list == NULL) {
		return ERROR_EXIT;
	}
	else if (freeNodes == NULL) {
		return ERROR_EXIT;
	}
	else if (list->count == 0) {
		nextFree = (NODE*)freeNodes->prev;
		
		list->head = freeNodes;
		list->tail = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes->item = item;
		freeNodes->next = NULL;
		freeNodes->prev = NULL;

		freeNodes = nextFree;
		
		return SUCCESS_EXIT;
	}
	else {
		NODE* currentTail;
		
		nextFree = (NODE*)freeNodes->prev;

		currentTail = list->tail;
		currentTail->next = freeNodes;
		
		freeNodes->item = item;
		freeNodes->prev = currentTail;
		freeNodes->next = NULL;
	
		list->tail = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;

		freeNodes = nextFree;

		return SUCCESS_EXIT;
	}
}

/*
 * Adds an item to the beggining of the list
 * @param list: The list we are adding to
 * @param item: The item we are going to add to our list
 * @return 0 if the item was successfully added, -1 if adding the item
 * failed
 */
int ListPrepend(LIST* list, void* item) {
	NODE* nextFree;
	if (item == NULL) {
		return ERROR_EXIT;
	}
	else if (list == NULL) {
		return ERROR_EXIT;
	}
	else if (freeNodes == NULL) {
		return ERROR_EXIT;
	}
	else if (list->count == 0) {
		nextFree = (NODE*)freeNodes->prev;
		
		list->head = freeNodes;
		list->tail = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;
		
		freeNodes->item = item;
		freeNodes->next = NULL;
		freeNodes->prev = NULL;

		freeNodes = nextFree;
			
		return SUCCESS_EXIT;
	}
	else {
		NODE* currentHead;

		nextFree = (NODE*)freeNodes->prev;

		currentHead = list->head;
		currentHead->prev = freeNodes;

		freeNodes->item = item;
		freeNodes->next = currentHead;
		freeNodes->prev = NULL;

		list->head = freeNodes;
		list->curr = freeNodes;
		list->count = list->count + 1;

		freeNodes = nextFree;
		
		return SUCCESS_EXIT;
	}
}
