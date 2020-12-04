#ifndef DC_REQUEST_H
#define DC_REQUEST_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../dc_lib/unistd.h"

#define BUF_SIZE 4096

int get_method(char request[]);

int parse_request(char request[], char file_name[], int request_len);

#endif //DC_REQUEST_H
