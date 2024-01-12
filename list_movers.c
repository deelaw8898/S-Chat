
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <string.h>

/*
 * Returns the current number of items in list
 * @param list: list we want to find the count of
 * @return the number of items in our list
 */
int ListCount(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot get count of a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else {
		return list->count;
	}
}

/*
 * Moves cursor to the first item in our list, and returns that item
 * @param list: The list we are navigating
 * @return the item in the first position of our list, if no head
 * (empty list), return null
 */ 
void* ListFirst(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot navigate a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->head == NULL) {
		return NULL;
	}
	else {
		list->curr = list->head;
		return list->curr->item;
	}
}

/*
 * Moves cursor to the last position in list and returns the item
 * @param list: list we are navigating
 * @return the item in the last position of our list. If tail is null
 * (empty list), just return null pointer
 */  
void* ListLast(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot navigate a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->tail == NULL) {
		return NULL;
	}
	else {
		list->curr = list->tail;
		return list->curr->item;
	}
}

/*
 * Attemps to move the cursor to the next item in our list and returns the
 * item
 * @param list: the list we are navigating
 * @return the item at our new cursor positon, or null if we attempt to
 * navigate past the end of the list. If the cursor is null, will move 
 * forward to the head of the list
 */ 
void* ListNext(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot navigate a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->curr == NULL) {
		list->curr = list->head;
		if (list->head == NULL) {
			return NULL;
		}
		else {
			return list->head->item;
		}
	}
	else if (list->curr->next == NULL) {
		return NULL;
	}
	else {
		list->curr = (NODE*) list->curr->next;
		return list->curr->item;
	}
}

/*
 * Attemps to move the cursor to the previous item in our list and returns
 * the item
 * @param list: the list we are navigating
 * @return the item at our new cursor positon, or NULL if we attempt to move
 * backwards past the start of list. If our cursor is null, will move back
 * to the tail item  
 */
void* ListPrev(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot navigate a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->curr == NULL) {
		return list->tail;
	}
	else if (list->curr->prev == NULL) {
		return NULL;
	}
	else {
		list->curr = (NODE*) list->curr->prev;
		return list->curr->item;
	}
}

/*
 * Returns the item at our current cursor position
 * @param list: the list we are retrieving the item from
 * @return the item at our current cursor position, if cursor is
 * NULL, simply return NULL pointer
 */
void* ListCurr(LIST* list) {
	if (list == NULL) {
		char* msg = "Error, cannot get the cursor of a null list\n";
		fwrite(msg, 1, strlen(msg), stderr);
		exit(ERROR_EXIT);
	}
	else if (list->curr == NULL) {
		return NULL;
	}
	else {
        return list->curr->item;
    }
}

/*
 * Navigates our list, and searches for a match with comparison_arg, according
 * to the comparison algorithm defined by comp. When found, our cursor will
 * remain at the item that matched comparison_arg. If we reach the end of the
 * list, cursor remains at end
 * @param list: the list we will be searching
 * @param comp: a pointer to a function that determines whether the current
 * item matches comparison_arg
 * @param comparison_arg: item we will be searching for
 * @return the item that matches, or NULL if comparison_arg is not in the list
 */
void* ListSearch(LIST* list, Comparator comp, void* comparisonArg) {
	if ((*comp) (ListCurr(list), comparisonArg) == 0) {
		return ListCurr(list);
	}
	else {
		while(ListNext(list) != NULL) {
			if ((*comp)(ListCurr(list), comparisonArg) == 0) {
				return ListCurr(list);
			}
		}
	}
	
	return NULL;
}

