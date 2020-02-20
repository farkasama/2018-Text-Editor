#include <ncurses.h>
#include <string.h>
#include <stdio.h>

struct windows;

WINDOW *create_save_win(char *save_name, int row, int col);
int delete_win(WINDOW *win);
