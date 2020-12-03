#include "edit-config.h"

int y_offset = 0;
int tab_offset = 0;

#define DEBUG

// Main driver of the program.
int main(int argc, char *argv[]) {
    // Initialize a new page
	PAGE page;

	// Loads the config file or creates one if it doesn't exist.
    if(file_exists("../edit-config.txt")) {
        load_file(&page, "../edit-config.txt");
    } else {
        init_page(&page, "../edit-config.txt", PAGE_SIZE);
        page.numlines = 1;
    }

	// Curses interface.
	initscr(); // Initializes the screen. Uses first refresh operation to clear screen. Initializes all implementation data structures.
	noecho(); // Turns off AUTOMATIC echoing of keys to the terminal. Gives control and helps getch() (get character) function properly.
	keypad(stdscr, true); // FEATURE!! Enables F1, F2, etc...
	
	int beg = 0;
	int end = WIN_SIZE;
	int x, y; // position on screen
    int i;

	update_status("Press F4 to quit");
	
	print_page(&page, beg, end);
	getyx(stdscr, y, x); // Retries the curses cursor and coordinates.

    char status[NAME_LIMIT + 10];
	while(true) // Keeps looping and listens for key input.
	{
		beg = 0 + y_offset;
		end = WIN_SIZE + y_offset;
		int ch = getch(); // Getch is Get Character.
		update_status("Press F4 to quit, F5 to save and F6 for both."); // default text
		switch(ch)
		{
			case KEY_F(4):
			    goto endnc;
			case KEY_F(5):
				save_file(&page);
                sprintf(status, "Saved.", page.filename);
				update_status(status);
				break;
		    case KEY_F(6):
                save_file(&page);
                goto endnc;
            case KEY_UP:
				move_up(&page, &x, &y);
				break;
			case KEY_DOWN:
				move_down(&page, &x, &y);
				break;
			case KEY_LEFT:
				move_left(&x, &y);
				break;
			case KEY_RIGHT:
				move_right(&page, &x, &y);
				break;
			case KEY_DC:
			case 127: // Also a backspace key.
			case KEY_BACKSPACE:
				if(strlen(page.text[y + y_offset].line) == 0) {
					remove_line(&page, y + y_offset);
					move_up(&page, &x, &y);
					int counter = 0;
					while (page.text[y + y_offset + counter].line) {
					    counter++;
					}
					for (int i = 0; i < counter; i++) {
                        move_right(&page, &x, &y);
					}
				}
				else if( x > 1 ) {
					remove_char(&page.text[y + y_offset], x - 2); // delete
					move_left(&x, &y);				  // char behind cursor
				}
				print_page(&page, beg, end);
				move(y, x);
				break;
            case '\t': // Inserts a tab.
                for(i = 0; i < TAB_WIDTH; i++) {
                    insert_char(&page.text[y + y_offset], ' ', x - 1);
                    print_page(&page, beg, end);
                    move_right(&page, &x, &y);
                }
                break;
			case '\n': // Inserts new line.
				insert_line(&page, y + y_offset + 1);
				print_page(&page, beg, end);
				move_down(&page, &x, &y);
				break;
			default: // All other characters
				if( isprint(ch) ) { // Checks if the character is printable or not.
					insert_char(&page.text[y + y_offset], ch, x - 1);
					print_page(&page, beg, end);
					move_right(&page, &x, &y);
				}
		}
	}
endnc:	
	/* end curses */
    endwin();
	dest_page(&page);
	return EXIT_SUCCESS;
} // main

// prints a message at the bottom of the window
void update_status(char *info) {
	int oldy, oldx; getyx(stdscr, oldy, oldx);
	
	attron(A_REVERSE);
	move(LINES - 1, 0);
	clrtoeol();
	printw(info);
	attroff(A_REVERSE);
	
	move(oldy, oldx);
}

void move_left(int *x, int *y) {
	if (*x - 1 > 0) {
	    move(*y, --(*x));
	}
}

void move_right(PAGE *p, int *x, int *y) {
	if(*x <= strlen(p->text[*y + y_offset].line)) {
        if(p->text[*y + y_offset].line[*x + tab_offset] == '\t') {
            move(*y, ++(*x));
        } else {
            move(*y, ++(*x));
        }
    }
}

void move_up(PAGE *p, int *x, int *y) {
	if( *y > 0 ) {
		--(*y);
	} else if (y_offset > 0) {
		--(y_offset);
		print_page(p, 0 + y_offset, WIN_SIZE + y_offset);	
	}
	if( *x > strlen(p->text[*y + y_offset].line) + 1 ) // cursor adjusts
		*x = strlen(p->text[*y + y_offset].line) + 1;  // to smaller lines
	move(*y, *x);
}

void move_down(PAGE *p, int *x, int *y) {
	if( *y < WIN_SIZE - 1 && *y < p->numlines  - 1 ) {
		 ++(*y);
	} else if ( *y + y_offset < p->numlines - 1 ) {
		++(y_offset);
		print_page(p, 0 + y_offset, WIN_SIZE + y_offset);	
	}

	if( *x > strlen(p->text[*y + y_offset].line) + 1 )
		*x = strlen(p->text[*y + y_offset].line) + 1;
	move(*y, *x);
}

int count_lines(FILE *fp) {
	char ch = '\0';
	int count = 0;
	while((ch = fgetc(fp)) != EOF) {
		if( ch == '\n' ) {
			count++;
        }
    }
    fseek(fp, 0, SEEK_SET); // go to beginning of file
	return count;
}

void load_file(PAGE *p, char *filename) {
	FILE *fp = fopen(filename, "r");
	int size = count_lines(fp) * 2;
	char ch = '\0';
	int line;

    if(size < PAGE_SIZE) {
        size = PAGE_SIZE;
    }

	init_page(p, filename, size);

    if(fp == NULL) { // File doesn't exist yet. don't bother reading
        p->numlines = 1;
        return;
    }

    for(line = 0; line < size && ch != EOF; line++) {
        ch = fgetc(fp);
        while(ch != '\n' && ch != EOF) {
            LINE *currline = &(p->text[line]);
            if(ch != '\t') {
                add_char(currline, ch);
            } else {
                for(int i = 0; i < TAB_WIDTH; i++) {
                    add_char(currline, ' ');
                }
            }
            ch = fgetc(fp);
        }
        p->numlines++;
    }
	fclose(fp);
}

void save_file(PAGE *p) {
	FILE *fp = fopen(p->filename, "w");
	int line, col;

	for(line = 0; line < p->numlines; line++) {
		col = 0;
		while(p->text[line].line[col] != '\0') {
			fputc(p->text[line].line[col], fp);
			col++;
		}
		fputc('\n', fp);
	}
	fclose(fp);
} // save_file

int file_exists(char *filename) {
    FILE *fp = fopen(filename, "r");
    if(fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
} // saving and loading
