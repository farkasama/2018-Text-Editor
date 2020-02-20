#ifndef __DLINKEDLIST_H__
#define __DLINKEDLIST_H__

#include <stdlib.h>
#include <stdio.h>

struct DLinkedList
{
	struct Node *first;
	struct Node *current;
	struct Node *last;
	int size;
	int curPos;
};

struct Node
{
	void *data;
	struct Node *next;
	struct Node *previous;
};
struct Node* new_Node(void *data);

struct DLinkedList* new_DLinkedList();


//careful!, do SEG FAULT if list is empty
int get_curPos(struct DLinkedList *list);

void* get_data(struct DLinkedList *list);

//retroune la taille de la liste
int get_size(struct DLinkedList *list);

//retourne si il y a un un argument apres
int has_next(struct DLinkedList *list);

//retourne si il y a un un argument avant
int has_previous(struct DLinkedList *list);

//retourne si la liste est vide
int isEmpty(struct DLinkedList *list);

//add data after current, current position move to new data
void add(struct DLinkedList *list, void *data);

//add data before current, current position not changed
void add_previous(struct DLinkedList *list, void *data);

//add data after current, current position not changed
void add_next(struct DLinkedList *list, void *data);

//if list has next -> go next. return 1 is sucess
int go_next(struct DLinkedList *list);

//if list has previous -> go previous. return 1 if sucess
int go_previous(struct DLinkedList *list);

//va a la tete de la liste
int go_head(struct DLinkedList *list); 

//va la a la queu de la liste
int go_tail(struct DLinkedList *list); 

/*If previous is not NULL move to previous after remove, 
  else move to next
  */
void rm_current(struct DLinkedList *list);

//supprime l'argument precedent
void rm_previous(struct DLinkedList *list);

//suprime le prochain argument
void rm_next(struct DLinkedList *list);

//concatenate two list to one, src will be free
void concat(struct DLinkedList **desc, struct DLinkedList **src);

//separate into src = [:current] and return [current:]
//inclusion: 1 to include current, 0 else
struct DLinkedList* separe(struct DLinkedList **src, int inclusion);

//free all node and self;
void free_DLinkedList(struct DLinkedList *list);

#endif
