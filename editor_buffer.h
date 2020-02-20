#ifndef __EDITOR_BUFFER_H__
#define __EDITOR_BUFFER_H__

#include <stdlib.h>
#include <stdio.h>
#include "DLinkedList.h"

extern char *ascii;

struct editor_buffer;

struct editor_buffer
{
	struct DLinkedList *lines;
	struct DLinkedList *chars;
	int CursorPos;
};

//initialise le buffer
struct editor_buffer* new_buffer();

void* new_char(char value);

void free_buff(struct editor_buffer *buff);


/* 
 * Renvoie 1 si on peut bouger le cursor 
 * 	vers la droite sans changer de ligne
 * 0 sinon
 */
int has_right(struct editor_buffer *buff);


/* 
 * Renvoie 1 si on peut bouger le cursor 
 * 	vers la gauche sans changer de ligne
 * 0 sinon
 */
int has_left(struct editor_buffer *buff);

// Renvoie 1 si il a une ligne apres current 0 sinon
int has_line_right(struct editor_buffer *buff);

// Renvoie 1 si il a une ligne qui precede current 0 sinon
int has_line_left(struct editor_buffer *buff);

int go_begin_line(struct editor_buffer *buff);

int go_end_line(struct editor_buffer *buff);

int get_lines_size(struct editor_buffer *buff);

int get_chars_size(struct editor_buffer *buff);

int get_buffx(struct editor_buffer* buff);

int get_buffy(struct editor_buffer* buff);

// Fill x and y with the position in the buffer
int get_buffxy(struct editor_buffer* buff,  int *x, int *y);

char get_char(struct editor_buffer *buff);

//standard type
void add_char(struct editor_buffer *buff, unsigned char data);

/* 	for enter key
	Split en deux lignes du cursor courant
*/
void add_line(struct editor_buffer *buff);


void add_str(struct editor_buffer *buff, char *str, int size);

/*
	move cursor one char right
	1 if moved
*/
int cur_right(struct editor_buffer *buff);

//move cursor one char left
int cur_left(struct editor_buffer *buff);

/*
 * move cursor upper line
 * 0 if no move or just up
 * negative value if move to left
 */
int cur_up(struct editor_buffer *buff);

//move cursor lower line
int cur_down(struct editor_buffer *buff);

//for backspace key
int rm_char(struct editor_buffer *buff);

//for suppr key
int rm_right_chars(struct editor_buffer *buff);

//supprime la ligne courrante
void rm_line(struct editor_buffer *buff); //Non fait

//renvoie la taille de la selection
int get_selection_size(struct editor_buffer *buff, int beginX, int beginY);

//renvoie la selection sous forme de char*
int get_selection_str(struct editor_buffer *buff,  
	int beginX, int beginY, int endX, int endY, char *ret);

int movetox(struct editor_buffer *buff, int x);

int movetoxy(struct editor_buffer* buff, int x, int y);

int getnstrteol(struct editor_buffer *buff, char *res, int taille);

//affiche le buff
void print_buff(struct editor_buffer *buff);

#endif