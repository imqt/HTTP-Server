//
// Created by hans on 2020-11-21.
//

#ifndef DC_EDITCONFIG_H
#define DC_EDITCONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <ncurses.h>
#include <signal.h>

#include "ncurses/page.h"

void update_status(char* info);

int count_lines(FILE* fp);
void load_file(page* page, char* file);
//void save_file(page* page);
int file_exists(char* file);

//void move_left(int* x, int* y);
//void move_right(page* page, int* x, int* y);
//void move_up(page* page, int* x, int* y);
//void move_down(page* page, int* x, int* y);

#endif //DC_EDITCONFIG_H
