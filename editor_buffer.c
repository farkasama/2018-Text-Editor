#include "editor_buffer.h"
#define _get_cursor(buff) (buff->CursorPos)
#define C_LEFT 0
#define C_RIGHT 1

char *ascii = 
"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\
\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\
\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\
\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\
\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\
\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\
\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\
\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\
\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\
\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\
\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\
\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\
\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\
\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\
\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\
\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff";

struct editor_buffer* new_buffer()
{
	struct editor_buffer *buff = malloc(sizeof(struct editor_buffer));
	buff->CursorPos = C_LEFT;
	buff->lines = new_DLinkedList();
	buff->chars = new_DLinkedList();
	add(buff->lines, buff->chars);
	return buff;
}

void free_buff(struct editor_buffer *buff)
{
	if(buff == NULL)
		return;
	else if (buff->lines == NULL)
	{	
		free(buff);
		return;
	}
	struct Node *iter_line = buff->lines->first;
	while(iter_line != NULL)
	{	
		free_DLinkedList((struct DLinkedList*)(iter_line->data));
		iter_line = iter_line->next;
	}
	free_DLinkedList(buff->lines);
	free(buff);
}

inline void* new_char(char value)
{
	char *res = malloc(sizeof(char*));
	*res = value; 
	return res;
}

inline int has_right(struct editor_buffer *buff)
{
	if (buff->CursorPos == C_LEFT) return 1;
	else return has_next(buff->chars);
}

inline int has_left(struct editor_buffer *buff)
{
	if (!has_previous(buff->chars))
	{
		if (buff->CursorPos == C_LEFT)
			return 0;
	}
	return 1;
}

inline int has_line_right(struct editor_buffer *buff)
{
	return has_next(buff->lines);
}

inline int has_line_left(struct editor_buffer *buff)
{
	return has_previous(buff->lines);
}

int go_end_line(struct editor_buffer *buff)
{
	if (buff == NULL) return 0;
	go_tail(buff->chars);
	buff->CursorPos = C_RIGHT;
	return 1;
}

int go_begin_line(struct editor_buffer *buff)
{
	if (buff == NULL) return 0;
	go_head(buff->chars);
	buff->CursorPos = C_LEFT;
	return 1;
}

int get_lines_size(struct editor_buffer *buff)
{
	return get_size(buff->lines);
}

int get_chars_size(struct editor_buffer *buff)
{
	return get_size(buff->chars);
}

int get_buffx(struct editor_buffer* buff)
{
	int x = 0;
	if (buff == NULL) return 0;
	if (get_size(buff->chars)==0) return 0;
	else x = get_curPos(buff->chars);
	if (buff->CursorPos == C_RIGHT) x+=1;
	return x;
}

int get_buffy(struct editor_buffer* buff)
{
	if (buff == NULL) return 0;
	return get_curPos(buff->lines);
}

int get_buffxy(struct editor_buffer* buff,  int *x, int *y)
{
	*x = get_buffx(buff);
	*y = get_buffy(buff);
	if (!x || !y) return 0;
	else return 1;
}

char get_char(struct editor_buffer *buff)
{
	if (get_size(buff->chars) == 0) return -1;
	return *(char *)get_data(buff->chars);
}

void add_char(struct editor_buffer *buff, unsigned char data)
{
	void *new = &ascii[data];
	if (_get_cursor(buff) == C_LEFT)
	{
		add_previous(buff->chars, new);
		go_previous(buff->chars);
		buff->CursorPos = C_RIGHT;
	}
	else 
	{
		add(buff->chars, new);
	}
}

void add_str(struct editor_buffer *buff, char *str, int size)
{
  for (int i = 0; i < size; i++) {
    if(str[i] == '\n')
      add_line(buff);
    else
      add_char(buff,str[i]);
  }
}

void add_line(struct editor_buffer *buff)
{
	int inclusion;
	if (buff->CursorPos == C_RIGHT)
		inclusion = 0;
	else
		inclusion = 1;
	struct DLinkedList* new_line = 
		separe((struct DLinkedList**)(&buff->lines->current->data), inclusion);
	add(buff->lines, new_line);
	buff->chars = new_line;
	buff->CursorPos = C_LEFT;
}

int cur_right(struct editor_buffer* buff)
{
	if (buff->CursorPos == C_LEFT && get_size(buff->chars))
	{
		buff->CursorPos = C_RIGHT;
		return 1;
	} else if (go_next(buff->chars))
		return 1;
	else if (go_next(buff->lines))
	{
		buff->chars = (struct DLinkedList *)get_data(buff->lines);
		if (go_head(buff->chars))
			buff->CursorPos = C_LEFT;
		return -1 -get_curPos(buff->chars);
	}
	else
		return 0;
}

int cur_left(struct editor_buffer* buff)
{
	if (go_previous(buff->chars))
		return 1;
	else if (buff->CursorPos == C_RIGHT)
	{
		buff->CursorPos = C_LEFT;
		return 1;
	}
	else if (go_previous(buff->lines))
	{
		buff->chars = (struct DLinkedList *)get_data(buff->lines);
		if (go_tail(buff->chars))
			buff->CursorPos = C_RIGHT;
		return -1 -get_curPos(buff->chars);
	}
	else 
		return 0;
}

int cur_up(struct editor_buffer *buff)
{
	int nb_move = 0;
	if (has_line_left(buff))
	{
		go_previous(buff->lines);
		struct DLinkedList *prev_line = 
			(struct DLinkedList*)get_data(buff->lines);
		buff->chars = prev_line;
		buff->CursorPos = C_LEFT;
		go_begin_line(buff);
		nb_move = 1;
	}
	return nb_move;
}

int cur_down(struct editor_buffer *buff)
{
	int nb_move = 0;
	if (has_line_right(buff))
	{
		go_next(buff->lines);		
		struct DLinkedList *nextline_chars = 
			(struct DLinkedList*)get_data(buff->lines);
		buff->chars = nextline_chars;
		buff->CursorPos = C_LEFT;
		go_begin_line(buff);
		nb_move = 1;
	}
	return nb_move;
}


int rm_char(struct editor_buffer *buff)
{
	int move = 1;
	if (_get_cursor(buff) == C_RIGHT)
	{
		if (!has_previous(buff->chars)){
			buff->CursorPos = C_LEFT;
		}
		rm_current(buff->chars);
	}
	else if (go_previous(buff->lines))
	{
		if (get_size((struct DLinkedList*)get_data(buff->lines))==0)
		{
			free(get_data(buff->lines));
			rm_current(buff->lines);
			go_next(buff->lines);
			move = 0;
		}
		else 
		{
			struct DLinkedList *prevline_chars = (struct DLinkedList*)get_data(buff->lines);
			go_tail(prevline_chars);
			buff->CursorPos = C_RIGHT;
			concat(&prevline_chars,&(buff->chars));
			buff->chars = prevline_chars;
			rm_next(buff->lines);
			move = -get_size(buff->chars);
		}
	} else move = 0;
	return move;
}

int rm_right_chars(struct editor_buffer *buff)
{
	int res = 0;
	if (has_right(buff)) res = 1;
	else if (has_line_right(buff)) res = -1;
	else return res;
	cur_right(buff);
	rm_char(buff);
	return res;
}

int get_selection_size(struct editor_buffer *buff, int beginX, int beginY)
{
	int endX, endY;
	get_buffxy(buff,&endX, &endY);
	int nb_lines = endY - beginY +1;
	if (nb_lines <= 0) return 0;
	int total_chars = 0;
	if (nb_lines == 1)
		total_chars += endX-beginX;
	else
	{
		struct Node *iter_lines = buff->lines->current;
		total_chars += get_curPos((struct DLinkedList*)iter_lines->data)+1;
		iter_lines = iter_lines->previous;
		endY--;
		while (endY > beginY && iter_lines != NULL)
		{
			total_chars += get_size((struct DLinkedList*)iter_lines->data);
			iter_lines = iter_lines->previous;
			endY--;
		}
		if (iter_lines != NULL)
			total_chars += get_curPos((struct DLinkedList*)iter_lines->data)
						+  1-beginX;
	}	
	total_chars += nb_lines-1; /* n-1 '\n' */
	return total_chars;
}


int get_selection_str(struct editor_buffer *buff,  
	int beginX, int beginY, int endX, int endY, char *ret)
{
	int tmp;
	if (beginY > endY || (beginY == endY && beginX > endX)) {
		tmp = endY; endY = beginY; beginY = tmp; //swap beginY and endY
		tmp = endX; endX = beginX; beginX = tmp; //swap beginX and endX
	}
	movetoxy(buff, endX, endY);
	int total_chars = get_selection_size(buff, beginX, beginY);
	ret = malloc(sizeof(char)*total_chars+1);
	struct Node *cur_chars_flag = buff->chars->current;
	struct Node *cur_lines_flag = buff->lines->current;
	for (int i = total_chars-1; i >= 0; i--)
	{
		if (buff->CursorPos == C_LEFT)
		{
			// printf("\n");
			ret[i] = '\n';
			cur_left(buff);
		}
		else
		{
			ret[i] = *(char*)get_data(buff->chars);
			// printf("%c",*(char*)get_data(buff->chars));
			cur_left(buff);
		}
	}
	buff->chars->current = cur_chars_flag;
	buff->lines->current = cur_lines_flag;
	ret[total_chars] = '\0';
	return total_chars;
}

static int movetoy(struct editor_buffer *buff, int y)
{
	int buff_size = get_lines_size(buff);
	if ( buff_size <= y) return 0;
	struct Node* iter_line = buff->lines->first;
	int i;
	for ( i = 0; i < y ; i++)
	{
		iter_line = iter_line->next;
	}
	buff->lines->current = iter_line;
	buff->lines->curPos = i;
	return 1;
}

int movetox(struct editor_buffer *buff, int x)
{
	int buff_size = get_chars_size(buff);
	if ( buff_size <= x) return 0;
	go_begin_line(buff);
	int i;
	for ( i = 0; i < x ; i++)
	{
		cur_left(buff);
	}
	return 1;
}

int movetoxy(struct editor_buffer* buff, int x, int y)
{
	if (!movetoy(buff, y) && !movetox(buff, x)) return 0;
	return 1;
}

int getnstrteol(struct editor_buffer *buff, char *res, int taille)
{
	struct Node* iter_char = buff->chars->current;
	int count = 0;
	if (taille <= 0 || iter_char == buff->chars->last) return 0;
	else if (iter_char->next == NULL) {
		res[count++] = *(char*) iter_char->data;
		res[1] = '\0';
		return 1;
	}
	if (buff->CursorPos == C_LEFT) {
		res[count++] = *(char*) iter_char->data;
		if (iter_char->next == NULL) {
			res[1] = '\0';
			return 1;
		}
	}
	do {
		iter_char = iter_char->next;
		res[count] = *(char*) iter_char->data;
		count ++;
	} while(iter_char->next != NULL && count < taille-1);
	res[count] = '\0';
	return count;
}

void print_buff(struct editor_buffer *buff)
{	
	if(buff == NULL || buff->lines == NULL)
	{
		printf("buff vide \n");
		return;
	}
	struct Node *iter_line = buff->lines->first;
	struct Node *iter_char;
	int count = 1;
	while(iter_line != NULL)
	{	
		iter_char = ((struct DLinkedList*)(iter_line->data))->first;
		if (iter_line == buff->lines->current)
		{
			printf("[%d]: " ,count);
			if (iter_char == NULL) printf("|");
			while(iter_char != NULL)
			{
				if (iter_char == buff->chars->current)
				{	
					if (buff->CursorPos == C_LEFT)
						printf("|%c", *(char *)iter_char->data);
					else 
						printf("%c|", *(char *)iter_char->data);
				}
				else
				{
					printf("%c", *(char *)iter_char->data);
				}
				iter_char = iter_char->next;
			}
		}
		else
		{
			printf(" %d : " ,count);
			while(iter_char != NULL)
			{
				printf("%c", *(char *)iter_char->data);
				iter_char = iter_char->next;
			}
		}
		count++;
		printf("\n");
		iter_line = iter_line->next;
	}
}
