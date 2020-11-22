//
// Created by hans on 2020-11-21.
//

#ifndef DC_PAGE_H
#define DC_PAGE_H

#include <ncurses.h>
#include "line.h"

#define PAGE_SIZE 500
#define WIN_SIZE (LINES - 2)
#define NAME_LIMIT 256

typedef struct {
    char file_name[NAME_LIMIT];
    line *text;
    int number_of_lines;
    int size;
} page;

void init_page(page* page, char* filename, int size);
void destroy_page(page* page);
void insert_line(page* page, int index);
void remove_line(page* page, int index);
void print_page(const page* page, int start, int end);

#endif //DC_PAGE_H
