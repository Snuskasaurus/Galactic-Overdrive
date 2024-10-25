#include "List.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

ListElement *GetLastElement(List *_list);
ListElement *GetElementFromIndex(List *_list, int _index);
ListElement *GetPreviousElement(List *_list, ListElement *_element);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

void List_Destroy(List *_list)
{
    ListElement *current = _list->first;
    while (current)
    {
        _list->first = current->next;
        free(current->data);
        free(current);
        current = _list->first;
    }
    _list->first = NULL;
    _list->last = NULL;
    _list->number = 0;
}

// -- Move an element indentified by his pointer to the indicated index
void List_Move_PointerToIndex(List* _list, ListElement* _element, int _index)
{
	if (_element)
	{
		// -- Extract _element from the list ---------------------- //
		if (_element != _list->first)
		{
			ListElement* ptrPrevious = GetPreviousElement(_list, _element);
			if (_element == _list->last)
			{
				ptrPrevious->next = NULL;
				_list->last = ptrPrevious;
			}
			else
			{
				ptrPrevious->next = _element->next;
			}
		}
		else
		{
			_list->first = _element->next;
		}

		// -- Put back _element in the list at the _index ---------- //
		if (_index > 0)
		{
			if (_index >= _list->number)
			{
				_list->last->next = _element;
				_list->last = _element;
				_list->last->next = NULL;
			}
			else
			{
				// -- Get the element positioned just one index lower
				ListElement* indexPrevious = NULL;
				if (_index == 1)
				{
					indexPrevious = _list->first;
				}
				else if (_index < _list->number)
				{
					indexPrevious = GetElementFromIndex(_list, _index - 1);
				}

				_element->next = indexPrevious->next;
				indexPrevious->next = _element;
			}
		}
		else
		{
			_element->next = _list->first;
			_list->first = _element;
		}
	}
}

ListElement *List_InsertElement(List *_list, int _index, void *_data)
{
	ListElement *newElement = calloc(1, sizeof(ListElement));
	ListElement *current = NULL;

	if (newElement)
	{
		if (_list->number == 0)
		{
			_list->first = newElement;
			_list->last = newElement;
		}
		else
		{
			if (_index == 0)
			{
				newElement->next = _list->first;
				_list->first = newElement;
			}
			else if (_index >= _list->number)
			{
				_list->last->next = newElement;
				_list->last = newElement;
			}
			else
			{
				newElement->next = GetElementFromIndex(_list, _index);
				GetPreviousElement(_list, GetElementFromIndex(_list, _index))->next = newElement;
			}
		}
		newElement->data = _data;
		_list->number++;
	}
	return newElement;
}

void List_DestroyElementFromIndex(List *_list, int _index, int _imediate)
{
	ListElement *previous = NULL;
	ListElement *current = NULL;

	if (_imediate)
	{
		if (_index == 0)
		{
			current = _list->first;
		}
		else if (_index >= _list->number)
		{
			current = _list->last;
		}
		else
		{
			current = GetElementFromIndex(_list, _index);
		}
		List_DestroyElementFromPointer(_list, current, _imediate);
	}
	else
	{
		current = GetElementFromIndex(_list, _index);
		List_DestroyElementFromPointer(_list, current, _imediate);
	}
}

void List_DestroyElementFromPointer(List *_list, ListElement *_element, int _imediate)
{
	if (_element)
	{
		if (_imediate)
		{
			if (_list->number > 1)
			{
				if (_element == _list->first)
				{
					_list->first = _element->next;
				}
				else if (_element == _list->last)
				{
					_list->last = GetPreviousElement(_list, _element);
					_list->last->next = NULL;
				}
				else
				{
					GetPreviousElement(_list, _element)->next = _element->next;
				}
			}
			else
			{
				_list->first = NULL;
				_list->last = NULL;
			}
			free(_element->data);
			free(_element);
			_element = NULL;
			_list->number--;
		}
		else
		{
			_element->mustBeCleared = 1;
		}
	}
}

void List_Clear(List *_list)
{
	ListElement *current = _list->first;
	ListElement *next = NULL;
	while (current)
	{
		next = current->next;
		if (current->mustBeCleared)
		{
			List_DestroyElementFromPointer(_list, current, 1);
		}
		current = next;
	}
}

void List_Printf(List *_list)
{
	ListElement *current = _list->first;
	int index = 0;
	printf("----------------------------- LINKED LIST ------------------------------\n");
	printf("%d ELEMENTS\n", _list->number);
	printf("%p FIRST ELEMENT ADDRESS\n", _list->first);
	printf("%p LAST ELEMENT ADDRESS\n", _list->last);
	if (_list->first)
	{
		printf("ID\tELEMENT ADDRESS\t\tNEXT ADDRESS\t\tDATA ADDRESS\n");
		while (current)
		{
			printf("%d\t%p\t%p\t%p\n", index, current, current->next, current->data);
			current = current->next;
			index++;
		}
	}
	printf("------------------------------------------------------------------------\n");
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

ListElement *GetLastElement(List *_list)
{
	ListElement *current = _list->first;
	ListElement *previous = NULL;
	while (current)
	{
		previous = current;
		current = current->next;
	}
	_list->last = previous;
	return _list->last;
}

ListElement *GetElementFromIndex(List *_list, int _index)
{
	ListElement *current = _list->first;
	int i = 0;
	do
	{
		current = current->next;
		i++;
	} while (i != _index);
	return current;
}

ListElement *GetPreviousElement(List *_list, ListElement *_element)
{
	ListElement *current = _list->first;
	ListElement *previous = NULL;
	while (current)
	{
		previous = current;
		current = current->next;
		if (current == _element)
		{
			return previous;
		}
	}
	return NULL;
}