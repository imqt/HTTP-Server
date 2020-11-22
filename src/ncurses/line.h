//
// Created by hans on 2020-11-21.
//

#ifndef DC_LINE_H
#define DC_LINE_H

#include <string.h>
#include <stdlib.h>

#define LINE_SIZE 128
#define TAB_WIDTH 4

typedef struct
{
    char * line;
    int size;
} line;

void init_line (line *string);
void insert_char(line *string, char character, int index);
void remove_char(line *string, int index);
void add_char(line *string, char character);


#endif //DC_LINE_H
