#include <ncurses.h>
#include <string.h>
#include "editor_buffer.h"
#include "fileio.h"
#include "window.h"


void init()
{
    initscr();
	keypad(stdscr, TRUE);
	raw();
	noecho();
	scrollok(stdscr, TRUE);
	clearok(stdscr, TRUE);
	start_color();
	init_pair(1,COLOR_BLACK,COLOR_MAGENTA);
}

void end()
{
	endwin();
}

int quit()
{
	move(30,0);
	printw("quit ? [y] for quit: ");
	int ch = getch();
	move(30,0);
	clrtoeol();
	if (keyname(ch)[0] == 'y' || keyname(ch)[0] == 'Y')
		return 1;
	else
		return 0;
}

void printw_buff(struct editor_buffer *buff)
{
	if(buff == NULL || buff->lines == NULL)
		return;
	struct Node *iter_line = buff->lines->first;
	struct Node *iter_char;
	while(iter_line != NULL)
	{
		iter_char = ((struct DLinkedList*)(iter_line->data))->first;
		while(iter_char != NULL)
		{
			printw("%c", *(char *)iter_char->data);
			iter_char = iter_char->next;
		}
		iter_line = iter_line->next;
		if (iter_line != NULL) printw("\n");
		refresh();
	}
}

int main(int argc, char * argv[]){
	init();
	int row,col; 	//nb de ligne et colone de la fenetre
	getmaxyx(stdscr,row,col);

	struct editor_buffer* buff = new_buffer();
	if (argc > 1)
	{
		read_buff(argv[1], buff);
		printw_buff(buff);
		refresh();
	}
	char *clipboard = NULL;
	
	int i = 1000;
	do {
		int ch = getch();
		int x, y;
		// int maxX, maxY;
		char *save_name = malloc(28);
		int i_save = 0;
		WINDOW *save_win;
		getyx(stdscr, y, x);
		// getmaxyx(stdscr, maxY, maxX);

		for (int i = 20; i < 30 ; i++ )
		{
				move(i, 0);
				clrtoeol();
		}
		move(20,0);
		printw_buff(buff);
		move(y,x);
		refresh();
		char copie, n, selected;
		int copieX1, copieY1, copieX2, copieY2, clipboard_size;
		if (strcmp(keyname(ch), "^C")==0) {
			getyx(stdscr, y,x);
		  attroff(COLOR_PAIR(1));
		  if (quit())
		    break;
			move(y,x);
		}
		//se déplace vers la gauche
		else if (ch == 260) {
		  ch = get_char(buff);
		  int curseur = cur_left(buff);
		  if (curseur == 1) {
		    if (copie)
		      mvprintw(y,x-1,"%c",ch);
		    move(y, x-1);
		  }
		  else if (curseur <= 0) {
		    move(y-1, x-curseur);
		  }
		}
		//se déplace vers la droite
		else if (ch == 261) {
		  if (has_right(buff) == 1) {
		    move(y, x+1);
		  }
		  else if (has_line_right(buff) == 1) {
		    if (copie)
		      n = 1;
		    else
		      move(y+1, 0);
		  }
		  cur_right(buff);
		  if (copie) {
		    ch = get_char(buff);
		    if (n) {
		      mvprintw(y+1,0,"%c",ch);
		      move(y+1,0);
		    } else {
		      mvprintw(y,x+1,"%c",ch);
		      move(y,x+1);
		    }
		    n = 0;
		  }
		}

		//se déplace vers le haut
		else if (ch == 259) {
			if (has_line_left(buff)) {
				cur_up(buff);
				move(y-1, 0);
			}
		}
		//se déplace vers le bas
		else if (ch == 258) {
			if (has_line_right(buff)) {
				cur_down(buff);
				move(y+1, 0);
			}
		}
		//ajouter caractère
		else if (ch >= 32 && ch <= 125) {
			add_char(buff, ch);
			if (has_right(buff) == 1) {
				insch(ch);
				move(y, x+1);
			}
			else addch(ch);
			int posX, posY;
			getyx(stdscr, posY, posX);

			move(20,0);
			clrtoeol();
			printw_buff(buff);
			move(posY,posX);
			refresh();
		}
		else if (ch == 10) {
			getyx(stdscr,y,x);
			move(y+1,x);
			insertln();
			move(y,x);
			addch(ch);
			getyx(stdscr,y,x);
			int strsize = get_chars_size(buff) - get_buffx(buff)+1;
			if (strsize)
			{
				char *str = malloc(sizeof(char)*strsize);
				if (getnstrteol(buff, str, strsize))
				{
					printw("%s", str);
				}
				free(str);
			}
			add_line(buff);
			move(y,x);
			refresh();
		}
		else if (ch == KEY_HOME)
		{
			go_begin_line(buff);
			move(y, 0);
		}
		else if (ch == KEY_END)
		{
			go_end_line(buff);
			x = get_buffx(buff);
			move(y, x);
		}
		else if (ch == KEY_BACKSPACE) {
			int r = rm_char(buff);
			
			move(y, x-1);
			delch();


			int posX, posY;
			getyx(stdscr, posY, posX);
			move(posY,posX);
			refresh();
		}
		//selection pour copie
		else if (ch == 23) {
			if (copie == 0) {
				get_buffxy(buff, &copieX1, &copieY1);
				copie = 1;
				attron(COLOR_PAIR(1));
				if (has_right(buff)) {
				    ch = get_char(buff);
				    mvprintw(y,x,"%c",ch);
				    move(y,x);
				}
			}
			else {
				get_buffxy(buff, &copieX2, &copieY2);
				copie = 0;
				attroff(COLOR_PAIR(1));
				selected = 1;
			}
		}
		else if (ch == 24) {
		  if (!copie && selected) {
		    clipboard_size = get_selection_str(buff,copieX1,copieY1,copieX2,copieY2,clipboard);
		    if(copieY1 < copieY2) {
		      mvprintw(copieY1,copieX1,"%s",clipboard);
		    } else if (copieY2 < copieY1) {
		      mvprintw(copieY1,copieX1,"%s",clipboard);
		    } else if (copieX1 < copieX2) {
		      mvprintw(copieY1,copieX1,"%s",clipboard);
		    } else {
		      mvprintw(copieY2,copieX2,"%s",clipboard);
		    }
		  }
		}
		else if (ch == 22) {
		  add_str(buff,clipboard,clipboard_size);
		  printw("%s",clipboard);
		}
		//fenêtre de sauvegarde
		else if (ch == KEY_F(12)) {
		  	save_win = create_save_win(save_name,row,col);
			mvwprintw(save_win,3,1,"%s",save_name);
		  	do {
				//pour l'instant, manipule des char* et doit être modifié
				ch = getch();
				if (ch == 27) {
					i_save = 0;
					delete_win(save_win);
					break;
				}
				else if (ch == 10) {
					*(save_name+i_save) = '\0';
					write_buff(save_name,buff);
					delete_win(save_win);
					break;
				}
				else if (ch == KEY_BACKSPACE) {
				  getyx(save_win,y,x);
				  if (x != 1) {
				    mvwprintw(save_win,3,x-1," ");
				    *(save_name+(--i_save)) = '\0';
				  }
				  mvwprintw(save_win,3,1,"%s",save_name);
				}
				else if (i_save < 27) {
					wprintw(save_win,"%c",ch);
					wrefresh(save_win);
					*(save_name+i_save++) = ch;
				}
				wrefresh(save_win);
			} while (1);
		}
		//printw("id: %d,value: %s\n",ch,keyname(ch));
		i--;
	} while(i>0);

	end();
	return 0;
}
