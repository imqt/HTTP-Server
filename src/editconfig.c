#include "editconfig.h"

static void finish(int sig);

void update_status(char *info) {
    int oldy, oldx; getyx(stdscr, oldy, oldx);

    attron(A_REVERSE);
    move(LINES - 1, 0);
    clrtoeol();
    printw(info);
    attroff(A_REVERSE);
    move(oldy, oldx);
}

int count_lines(FILE *fp) {
    char ch ='\0';
    int count = 0;
    while((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fseek(fp, 0, SEEK_SET);
    return count;
}

void load_file(page* page, char* file) {
    FILE* fp = fopen(file, "r");
    int size = count_lines(fp) * 2;
    char end = '\0';
    int which_line;

    if (size < PAGE_SIZE) {
        size = PAGE_SIZE;
        init_page(page, file, size);
    }

    if (fp == NULL) {
        page->number_of_lines = 1;
        return;
    }

    for (which_line = 0; which_line < size && end != EOF; which_line++) {
        end = fgetc(file);
        while(end != '\n' && end != EOF) {
            line* current_line = &(page->text[which_line]);
            if(end != '\t') {
                add_char(current_line, end);
            } else {
                for (int i = 0; i < TAB_WIDTH; i++) {
                    add_char(current_line, ' ');
                }
            }
            end = fgetc(fp);
        }
        page->number_of_lines += 1;
    }
    fclose(fp);
}

int file_exists(char* file) {
    FILE* fp = fopen(file, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}

main(int argc, char *argv[]) {
    page page;

    load_file(&page, "config.txt");

    initscr();
    noecho();
    keypad(stdscr, true);

    int beg = 0;
    int end = WIN_SIZE;
    int y, x;
    int i;
    print_page(&page, beg, end);

    while (true) {
        int c = getch();
    }

    endwin();
    destroy_page(&page);
}
