/* Addison McAuley,	amm637,		11295940
 * Ghanim Jamil,	ghj729,		11298913
 */

#ifndef __LIST_H__
#define __LIST_H__

#define ERROR_EXIT -1
#define SUCCESS_EXIT 0

typedef struct _node {
	void* item;
	void* next;
	void* prev;
} NODE;

typedef struct _list {
	NODE* head;
	NODE* tail;
	NODE* curr;
	int count;
	void* next;
	void* prev;
} LIST;


typedef void (*itemFree) (void*);

typedef int (*Comparator) (void*, void*);

LIST* ListCreate();

int ListCount(LIST* list);

void* ListFirst(LIST* list);

void* ListLast(LIST* list);

void* ListNext(LIST* list);

void* ListPrev(LIST* list);

void* ListCurr(LIST* list);

int ListAdd(LIST* list, void* item);

int ListInsert(LIST* list, void* item);

int ListAppend(LIST* list, void* item);

int ListPrepend(LIST* list, void* item);

void* ListRemove(LIST* list);

void ListConcat(LIST* list_1, LIST* list_2);

void ListFree(LIST* list, itemFree item_to_be_freed);

void* ListTrim(LIST* list);

void* ListSearch(LIST* list, Comparator comp, void* comparison_arg);

#endif
