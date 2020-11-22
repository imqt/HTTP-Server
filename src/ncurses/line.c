//
// Created by hans on 2020-11-21.
//

#include "line.h"

// Initializes the a line.
void init_line(line* line) {
    line->size = LINE_SIZE;
    line->line = (char *)malloc(LINE_SIZE * sizeof(char));
    line->line[0] = '\0';
}

// Inserts a character into the line.
void insert_char(line* string, char character, int index) {

    // Ensures the string size is long enough to add the char.
    if (strlen(string->line) >= string->size - 2) {
        int doubled_size = string->size * 2;
        char *temp = (char *)malloc(doubled_size * sizeof(char));
        strcpy(temp, string->line);
        string->line = temp;
        string->size = doubled_size;
    }

    // At index, pushes everything after down 1 spot.
    for (int i = strlen(string->line); i >= index; i--) {
        string->line[i+1] = string->line[i];
    }

    // Puts the char in the empty spot.
    string->line[index] = character;
}

// Delete a character.
void delete_char(line* string, int index) {
    int length = strlen(string->line);
    for (int i = index; i < length; i++) {
        string->line[i] = string->line[i + 1];
    }
}

void add_char(line* string, char character) {
    insert_char(string, character, strlen(string->line));
}