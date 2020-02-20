#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <ncurses.h>
#include "editor_buffer.h"

void ncurses()
{
	initscr();              
	keypad(stdscr, TRUE);
	raw();
	noecho();
	scrollok(stdscr, TRUE);
	clearok(stdscr, TRUE);
	int row, col;
	getmaxyx(stdscr,row,col);
	int x, y;
	getyx(stdscr, y, x);
	do
	{
		int ch = getch();
		getyx(stdscr, y, x);
		if (strcmp(keyname(ch), "^C")==0) {
			printw("%d", ch);
			break;
		} else {
			printw("%d : %s\n", ch, keyname(ch));
		}
		refresh();
	} while (1);
	endwin();
}

int main(void)
{
	ncurses();
	return 0;
}