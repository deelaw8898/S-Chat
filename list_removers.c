
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <string.h>

extern LIST listMemory[];
extern NODE nodeMemory[];

extern int nextOpenList;
extern int nextOpenNode;

extern NODE* freeNodes;
extern LIST* freeLists;

/*
 * Removes the current item from the list, returns its item, and moves the
 * cursor to the next item
 * @preconditions: list cannot be empty
 * @param list: the list we are removing our item from
 * @return the item that has been removed
 */ 
void* ListRemove(LIST* list) {
	NODE* newFree;
	if (list == NULL) {
		char* msg = "Error, cannot remove from a NULL list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->count == 0) {
		char* msg = "Error, cannot remove from an empty list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->count == 1) {
		/*
		 * 1. Link current node to freeNodes
		 * 2. Set list fields to NULL/0
		 * 3. Update freeNodes to be our new free node
		 * 4. Return the item of freeNode
		 */
		newFree = list->curr;
		
		list->head = NULL;
		list->tail = NULL;
		list->curr = NULL;
		list->count = list->count - 1;

		freeNodes->next = newFree;
		newFree->prev = freeNodes;
		freeNodes = newFree;

		return newFree->item;	
	}
	else {
		/*
		 * 1. Get next and prev of current node
		 * 2. Relink next and prev to eachother
		 * 3. Unlink current node from list
		 * 4. Link current node to freeNodes
		 * 5. Update cursor to be our next item
		 * 6. If needed, update head/tail
		 * 7. Subtract count
		 * 8. Update freeNodes to be our new free node
		 * 9. Return item of freeNode
		 */
		NODE* currentNext;
		NODE* currentPrev;
		
		newFree = list->curr;

		currentNext = newFree->next;
		currentPrev = newFree->prev;

		/*
		 * Removing the tail - will make the cursor our new tail
		 * instead of the next item
		 */ 
		if (currentNext == NULL) {
			currentPrev->next = currentNext;
			list->curr = currentPrev;
			list->tail = currentPrev;
		}
		/*
		 * Removing the head - still set cursor to next, but need
		 * to update the list head reference
		 */ 
		else if (currentPrev == NULL) {
			currentNext->prev = currentPrev;
			list->curr = currentNext;
			list->head = currentNext;
		}
		/*
		 * Removing middle - simply just setting the cursor
		 */
		else {
			currentNext->prev = currentPrev;
			currentPrev->next = currentNext;
			list->curr = currentNext;
		}
		
		list->count = list->count - 1;

		newFree->prev = freeNodes;
		newFree->next = NULL;

	    if (freeNodes != NULL) {
		    freeNodes->next = newFree;
	    }	
		freeNodes = newFree;
		
		return newFree->item;
	}
}

/*
 * Deletes list, and frees all of its items
 * @param list: list to be deleted
 * @param itemFreeRoutine: pointer to a routine that will free an item from
 * our list
 */ 
void ListFree(LIST* list, itemFree itemFreeRoutine) {
	LIST* newFreeList;
    NODE* newFreeNode;
	if (list == NULL) {
		char* msg = "Error, cannot free a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	if (list->count != 0) {
		ListFirst(list);
	
		(*itemFreeRoutine)(ListCurr(list));
        
        newFreeNode = list->curr;
        newFreeNode->prev = freeNodes;
        
		while(ListNext(list) != NULL) {
		    (*itemFreeRoutine)(ListCurr(list));
		}
	    
        freeNodes = list->tail;

		list->head = NULL;
		list->tail = NULL;
		list->curr = NULL;
		list->count = 0;
	}
	newFreeList = list;

	newFreeList->prev = freeLists;
	newFreeList->next = NULL;
	
	if (freeLists != NULL) {
		freeLists->next = newFreeList;
	}	
	freeLists = newFreeList;	
}

/*
 * Removes the last item from the list, returns its item, and moves the
 * cursor to the new tail of the list
 * @preconditions: list cannot be empty
 * @param list: the list that we are trimming
 * @return the item that has been trimmed from our list
 */ 
void* ListTrim(LIST* list) {
	NODE* newFree;
	if (list == NULL) {
		char* msg = "Error, cannot trim a NULL list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->count == 0) {
		char* msg = "Error, cannot trim an empty list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->count == 1) {
		/*
		 * 1. Link current node to freeNodes
		 * 2. Set list fields to null/0
		 * 3. Update freeNodes to be new free node
		 * 4. Return item of free node
		 */
		newFree = list->tail;

		list->head = NULL;
		list->tail = NULL;
		list->curr = NULL;
		list->count = list->count - 1;

	    if (freeNodes != NULL) {
		    freeNodes->next = newFree;
	    }
		newFree->prev = freeNodes;
		freeNodes = newFree;

		return newFree->item;
	}
	else {
		/*
		 * 1. Get prev of current node
		 * 2. Unlink current from list
		 * 3. Link current node to freeNodes
		 * 4. Update cursor to be our new tail
		 * 5. Update tail to be new tail
		 * 6. Subtract count
		 * 7. update freeNodes to be new free node
		 * 8. Return item of freeNode
		 */
		NODE* currentPrev;

		newFree = list->tail;

		currentPrev = newFree->prev;

		currentPrev->next = NULL;
		list->curr = currentPrev;
		list->tail = currentPrev;

		list->count = list->count - 1;

		newFree->prev = freeNodes;
		newFree->next = NULL;

	    if (freeNodes != NULL) {
		    freeNodes->next = newFree;
	    }
		freeNodes = newFree;

		return newFree->item;
	}
}

/*
 * Concatenates list2 with list1
 * @param list1: The first list
 * @param list2: The second list
 * @postconditions: list2 will be deleted
 */
void ListConcat(LIST* list1, LIST* list2) {
	LIST* newFree;
	if (list1 == NULL) {
		char* msg = "Error, cannot concatenate to a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list2 == NULL) {
		char* msg = "Error, cannot concatenate a null list with a list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	/* If our first list is empty, will basically make our first list
	 * the second list. Will also maintain cursor of list 1 (NULL)
	 */	
	else if (list1->count == 0) {
		list1->head = list2->head;
		list1->tail = list2->tail;
		list1->count = list2->count;		
	}
	else {
		list1->tail->next = list2->head;
		if (list2->count != 0) {
			list2->head->prev = list1->tail;
			list1->tail = list2->tail;
		}
		list1->count = list1->count + list2->count;
	}
		list2->head = NULL;
		list2->tail = NULL;
		list2->curr = NULL;
		list2->count = 0;

		newFree = list2;

		newFree->prev = freeLists;
		newFree->next = NULL;

		if (freeLists != NULL) {
			freeLists->next = newFree;
		}

		freeLists = newFree;
}
