#include "line.h"

// Initializes a new line.
void init_line(LINE *string) {
    string->size = LINE_SIZE;
    string->line = (char *)malloc(LINE_SIZE * sizeof(char));
    string->line[0] = '\0';
}

// Inserts character into a string at a given index.
void insert_char(LINE *string, char c, int index) {
	if(strlen(string->line) >= string->size - 2) expand(string);

	for(int i = strlen(string->line); i >= index; i--) {
        string->line[i + 1] = string->line[i];
    }
	string->line[index] = c;
} // insert

// Appends character to a string.
void add_char(LINE *s, char c) {
    insert_char(s, c, strlen(s->line));
}

// Removes character from a string at a given index.
void remove_char(LINE *string, int index) {
		int len = strlen(string->line);
		for(int i = index; i < len; i++) {
			string->line[i] = string->line[i + 1];
        }
} // remove_char

// Doubles memory of a line.
void expand(LINE *string) {
	int new_size = string->size * 2;
	char *temp = (char *)malloc(new_size * sizeof(char));
	strcpy(temp, string->line);
	free(string->line);
	string->line = temp;
	string->size = new_size;
}