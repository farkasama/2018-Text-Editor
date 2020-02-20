#include "DLinkedList.h"

struct Node* new_node(void *data)
{
	struct Node* new = (struct Node*) malloc(sizeof(struct Node));
    new->data  = data;
	new->next = NULL;
	new->previous = NULL;
    return new;
}

struct DLinkedList* new_DLinkedList()
{
	struct DLinkedList* list = malloc(sizeof(struct DLinkedList));
	list->current = NULL;
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
	list->curPos = -1;
	return list;
}

inline int get_curPos(struct DLinkedList *list)
{
	return list->curPos;
}

void* get_data(struct DLinkedList *list)
{
	return list->current->data;
}

inline int get_size(struct DLinkedList *list)
{
	return list->size;
}

inline int isEmpty(struct DLinkedList *list)
{	
	if (list->current == NULL)
		return 1;
	else 
		return 0;
}

static inline void _go_next(struct DLinkedList *list)
{
	list->current = list->current->next;
	list->curPos++;
}

static inline void _go_previous(struct DLinkedList *list)
{
	list->current = list->current->previous;
	list->curPos--;
}

inline int has_next(struct DLinkedList *list)
{
	if (isEmpty(list) || list->current->next == NULL)
		return 0;
	else 
		return 1;
}

inline int has_previous(struct DLinkedList *list)
{
	if (isEmpty(list) || list->current->previous==NULL)
		return 0;
	else
		return 1;
}

void add(struct DLinkedList *list, void *data)
{
	add_next(list, data);
	if (has_next(list))
		_go_next(list);
}

void add_next(struct DLinkedList *list, void *data)
{
	struct Node *new = new_node(data);
	if (isEmpty(list))
	{
		list->first = new;
    	list->current = new;
		list->last = new;
		list->curPos++;
	}
    else if (!has_next(list))
    {	
    	list->current->next = new;
    	new->previous = list->current;
		list->last = new;
    }
    else
    {
    	new->next = list->current->next;
    	new->previous = list->current;
    	list->current->next->previous = new;
    	list->current->next = new;
    }
    list->size++;
}

void add_previous(struct DLinkedList *list, void *data)
{
	struct Node *new = new_node(data);

	if (isEmpty(list))
	{
		list->first = new;
    	list->current = new;
		list->last = new;
	}
    else if (!has_previous(list))
    {	
    	list->current->previous = new;
    	new->next = list->current;
		list->first = new;
    }
    else
    {
    	new->next = list->current;
    	new->previous = list->current->previous;
    	list->current->previous->next = new;
    	list->current->previous = new;
    }
    list->size++;
	list->curPos++;
}

int go_next(struct DLinkedList *list)
{
	if (has_next(list)){
		_go_next(list);
		return 1;
	}
	else
		return 0;
}


int go_previous(struct DLinkedList *list)
{
	if (has_previous(list)){
		_go_previous(list);
		return 1;
	}
	else
		return 0;
}

int go_head(struct DLinkedList *list)
{
	int ret = 0;
	if (get_size(list)!=0){
		list->current = list->first;
		ret = 1;
	} else list->curPos = -1;
	list->curPos = 0;
	return ret;
}

int go_tail(struct DLinkedList *list)
{
	int ret = 0;
	if (get_size(list)!=0) {
		list->current = list->last;
		ret = 1;
	}
	list->curPos = list->size-1;
	return ret;
}

void rm_current(struct DLinkedList *list)
{
	if (isEmpty(list))
		return;
	if (has_previous(list))
	{
		if(has_next(list))
		{
			list->current->previous->next = list->current->next;
			list->current->next->previous = list->current->previous;
			struct Node *tmp = list->current->previous;
			free(list->current);
			list->current = tmp;
		} 
		else 
		{
			list->current->previous->next = NULL;
			struct Node *tmp = list->current->previous;
			free(list->current);
			list->current = tmp;
			list->last = tmp;
		}
	}
	else
	{
		if (has_next(list))
		{
			list->current->next->previous = NULL;
			struct Node *tmp = list->current->next;
			free(list->current);
			list->current = tmp;
			list->first = tmp;
			return;
		}
		else
		{
			free(list->current);
			list->current = NULL;
			list->last = NULL;
			list->first = NULL;
		}
	}
	list->size--;
	list->curPos--;
}

void rm_previous(struct DLinkedList *list)
{
	if (has_previous(list))
	{
		_go_previous(list);
		rm_current(list);
		_go_next(list);
	}
}

void rm_next(struct DLinkedList *list)
{
	if(has_next(list))
	{
		_go_next(list);
		rm_current(list);
	}
}

void concat(struct DLinkedList **desc, struct DLinkedList **src)
{
	if (*desc == *src)
		return;
	else if (get_size(*src) == 0) {
		return;
	} else if (get_size(*desc) == 0){
		(*desc) = (*src);
	} else {
		// struct Node* cur = (*desc)->last;
		(*desc)->last->next = (*src)->first;
		(*src)->first->previous = (*desc)->last;
		(*desc)->last = (*src)->last;
		(*desc)->size += (*src)->size;
		// (*desc)->current = cur;
	}
	free(*src);
}

void free_DLinkedList(struct DLinkedList *list)
{
	if(!isEmpty(list))
	{
		list->current = list->first;
		while(has_next(list))
		{
			_go_next(list);
			free(list->current->previous);
		}
		free(list->current);
	}
	free(list);
}

struct DLinkedList* separe(struct DLinkedList **src,int inclusion)
{
	if(isEmpty(*src))
		return new_DLinkedList();
	else
	{
		struct DLinkedList *list_res;
		int curPos = get_curPos(*src);
		int size_src = get_size(*src);
		if(curPos == 0 && inclusion)
		{
			list_res= *src;
			*src = new_DLinkedList();
		}
		else if(curPos == size_src-1 && !inclusion)
			return new_DLinkedList();
		else
		{
			list_res= new_DLinkedList();
			list_res->last = (*src)->last;
			list_res->curPos = 0;
			if (inclusion)
			{
				list_res->first = (*src)->current;
				list_res->current = (*src)->current;
				list_res->size = size_src - curPos;
				(*src)->current = (*src)->current->previous;
				(*src)->last = (*src)->current;
				(*src)->size = curPos;
				(*src)->curPos--;

			}
			else
			{
				list_res->first = (*src)->current->next;
				list_res->current = (*src)->current->next;
				list_res->curPos = 0;
				list_res->size = size_src - curPos-1;
				(*src)->last = (*src)->current;
				(*src)->size = curPos+1;		
			}
			(*src)->last->next = NULL;
			list_res->first->previous = NULL;
		}
		return list_res;
	}
}