#include "fileio.h"


int exist_file(char* file_name)
{
    if( access( file_name, F_OK ) != -1 )
        return 1;
    else return 0;
}

int write_buff(char *file_name, struct editor_buffer *buff)
{
    FILE *fp;
    fp = fopen(file_name, "w");
    if(fp == NULL)
        return 0;
	if(buff == NULL || buff->lines == NULL)
	{
		return 0;
	}
	struct Node *iter_line = buff->lines->first;
	struct Node *iter_char;
	int count = 0;
	while(iter_line != NULL)
	{	
		iter_char = ((struct DLinkedList*)(iter_line->data))->first;
        while(iter_char != NULL)
        {
            fprintf(fp, "%c",*(char *)iter_char->data);
            iter_char = iter_char->next;
		    count++;
        }
		fprintf(fp, "\n");
		iter_line = iter_line->next;
	}

    fclose(fp);
    return count;
}

int read_buff(char *file_name, struct editor_buffer *buff)
{
    if (!exist_file(file_name)) return 0;
    int c;
    int count;
    FILE *file;
    file = fopen(file_name, "r");
    if (file) {
        while ((c = getc(file)) != EOF)
        {   
            if (c == '\n'){
                add_line(buff);
            } else {
                add_char(buff, c);
            }
            count ++;
        }
        fclose(file);
    }
    return count;
}