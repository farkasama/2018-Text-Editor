#include "window.h"

struct windows
{
	WINDOW *wr;
	WINDOW *menu;
	WINDOW *command;
};

WINDOW *create_save_win(char *save_name, int row, int col)
{
	WINDOW *save_win;
	save_win = newwin(5,30,(row-5)/2,(col-30)/2);
	box(save_win,0,0);
	mvwprintw(save_win,1,1,"Entrez un nom de fichier.");
	mvwprintw(save_win,3,1,"%s",save_name);
	wrefresh(save_win);
	return save_win;
}

int delete_win(WINDOW *win)
{
	mvwprintw(win,1,1,"\n\n\n");
	wborder(win,' ',' ',' ',' ',' ',' ',' ',' ');
	wrefresh(win);
	delwin(win);
	refresh();
	return 0;
}
