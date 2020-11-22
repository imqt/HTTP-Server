//
// Created by hans on 2020-11-21.
//

#include "page.h"

// Initializes empty page with given filename and size.
void init_page(page* page, char* filename, int size) {
    page->text = (line *)malloc(size * sizeof(line));

    for (int i = 0; i < size; i++) {
        init_line(page->text + i);
    }

    strcpy(page->file_name, filename);
    page->number_of_lines = 0;
    page->size = size;
}

// Yeets(destroys) the page by freeing each line in the text.
void destroy_page(page* page) {
    for (int i = 0; i < page->number_of_lines; i++) {
        free(page->text[i].line);
    }
    free(page->text);
}

void insert_line(page* page, int index) {
    if (page->number_of_lines >= page->size) {
        int doubled_size = page->size * 2;
        line* new_line = malloc(doubled_size * sizeof(line));

        for (int i = 0; i < page->size; i++) {
            new_line[i] = page->text[i];
        }
        for (int i = page->size; i < doubled_size; i++) {
            init_line(new_line + i);
        }
        page->text = new_line;
        page->size = doubled_size;
    }

    line inserted_line;
    init_line(&inserted_line);
    inserted_line.line[0] = '\0';

    for (int i = page->number_of_lines - 1; i >= index; i--) {
        page->text[i + 1] = page->text[i];
    }

    page->text[index] = inserted_line;
    page->number_of_lines += 1;
}


void remove_line(page* page, int index) {
    if(page->number_of_lines > 1) {
        free(page->text[index].line);

        for (int i = index; i < page->number_of_lines - 1; i++) {
            page->text[i] = page->text[i+1];
        }

        page->number_of_lines -= 1;
    }
}

// Prints page & puts cursor at end.
void print_page(const page *page, int start, int end) {

//    // Prints each line inside the page construct.
    int i, line;
    for (i = start, line = 0; i < page->number_of_lines && i < end; i++, line++) {
        move(line, 0);
        clrtoeol();
        printw(" %s", page->text[i].line);
    }

    // Moves cursor to the end of the displayed text.
    if (start < end) {
        move(line, 0);
        clrtoeol();
        move(line - 1, 1);
    }
    refresh();
}