#ifndef DC_REQUEST_H
#define DC_REQUEST_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../dc_lib/unistd.h"

#define BUF_SIZE 4096

int get_method(char request[]);

int find_last_dot_before_space(char request[], int start_index);

int parse_request(char request[], char file_name[], int* content_type_code, int request_len);

#endif //DC_REQUEST_H
