#ifndef LIST__H
#define LIST__H

// -- HEADERS -------------------------------------------------------------- //

#include <stdio.h>
#include <stdlib.h>

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct ListElement ListElement;
struct ListElement
{
	int mustBeCleared;
	ListElement *next;
	void *data;
};

typedef struct List List;
struct List
{
	int number;
	ListElement *first;
	ListElement *last;
};

// -- FUNCTIONS ------------------------------------------------------------ //

void List_Destroy(List *_list);

// -- Move an element indentified by his pointer to the indicated index
void List_Move_PointerToIndex(List* _list, ListElement* _element, int _index);

ListElement *List_InsertElement(List *_list, int _index, void *_data);

// Delete the element
void List_DestroyElementFromIndex(List *_list, int _index, int _imediate);

// Delete the element
void List_DestroyElementFromPointer(List *_list, ListElement *_element, int _imediate);

// Delete all the elements that must be cleaned
void List_Clear(List *_list);

void List_Printf(List *_list);

#endif // !LIST__H