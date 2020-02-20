#ifndef __FILEIO_H__
#define __FILEIO_H__

#include "editor_buffer.h"
#include <unistd.h>

int exist_file(char* file_name);

int write_buff(char * descr, struct editor_buffer *buff);

int read_buff(char *file_name, struct editor_buffer *buff);

#endif
