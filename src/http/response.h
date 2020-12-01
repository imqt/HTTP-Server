//
// Created by wuviv on 2020-11-04.
//

#ifndef DC_RESPONSE_H
#define DC_RESPONSE_H

#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../shared.h"
#include "../dc_lib/unistd.h"
#include "message.h"

void send_content(char file_name[], int cfd);

char * get_content_length(char file_name[]);

// Give the appropriate hardcoded status reponse.
// Helper for construct_response
// param: dest[]: a char array to store the status string
// param: status_code: http reponse status code (200, 400, 404,...)
void get_reason(char dest[], int status_code);

void respond(int cfd, char * file_name, int content_type_code, int request_code);

void construct_head(char response[], char *content_length, int status_code, int content_type_code);

// response[]          a place to put the response constructed
// *content            content to be sent for GET
// status_code 		   200 or some.. defined in response.c
// content_type_code   defined in shared.h
void construct_response(char response[], char *content_length, int status_code, int content_type_code);

#endif //DC_RESPONSE_H
