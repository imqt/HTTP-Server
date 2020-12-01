#include "page.h"

// Initializes a new page.
void init_page(PAGE *p, char *filename, int size) {
	p->text = (LINE *)malloc(size * sizeof(LINE));

	int i;
	for(i = 0; i < size; i++)
	{
		init_line(p->text + i);
	}
    strcpy(p->filename, filename);
	p->numlines = 0;
	p->size = size;
}

// Destroys the page. Frees memory.
void dest_page(PAGE *p) {
	for(int i = 0; i < p->numlines; i++) {
		free(p->text[i].line); // maybe replace with dest_line()
	}
	free(p->text);
}

// Inserts a new line.
void insert_line(PAGE *p, int index) {
	if( p->numlines >= p->size ) {
	    expand_page(p);
	}
	
	LINE newline;
	init_line(&newline);
	newline.line[0] = '\0';

	for(int i = p->numlines - 1; i >= index; i--) {
        p->text[i + 1] = p->text[i];
    }
	p->text[index] = newline;
	(p->numlines)++;
}

// Removes a line.
void remove_line(PAGE *p, int index) {
	if( p->numlines > 1 ) {
		free(p->text[index].line);

		for(int i = index; i < p->numlines - 1; i++) {
			p->text[i] = p->text[i + 1];
		}
		(p->numlines)--;
	}
}

// Increases memory allocation for the pages.
void expand_page(PAGE *p) {
	int newsize = p->size * 2;
	LINE *newline = malloc(newsize * sizeof(LINE));
	
	int i;
	// Copies old lines
	for(i = 0; i < p->size; i++) {
        newline[i] = p->text[i];
    }
	// Adds new lines
	for(i = p->size; i < newsize; i++) {
        init_line(newline + i);
    }

	p->text = newline;
	p->size = newsize;
}

// Prints the page. Moves cursor to end of text.
void print_page(const PAGE *p, int start, int end) {
	int i, line;
	for(i = start, line = 0; i < p->numlines && i < end; i++, line++) {
		move(line, 0);
		clrtoeol();
		printw(" %s", p->text[i].line);
	}
    if(start < end) {
        move(line, 0);   
        clrtoeol();
        move(line-1, 1);
    }
	refresh();
}
