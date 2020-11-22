#ifndef DC_REQUEST_H
#define DC_REQUEST_H

#include <string.h>
#include <stdio.h>

int parse_request(char request[], char file_name[], int* content_type_code);

#endif //DC_REQUEST_H
