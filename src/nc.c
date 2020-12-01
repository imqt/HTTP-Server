#include <ncurses.h>

int main() {
	initscr();
	// cbreak();
	raw();
	printw("Hellow world! %c", 65);
	addch('b');
	move(12, 13);
	mvprintw(25, 0, "Moved");
	mvaddch(26, 0, 'c');
	getch();
	endwin();
	return 0;
}