//
// Created by wuviv on 2020-11-04.
//

#ifndef DC_RESPONSE_H
#define DC_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return a dynamically allocated char array containing all characters from the given file.
char * get_content( char file_name[]);

// Give the appropriate hardcoded status reponse.
// Helper for construct_response
// param: dest[]: a char array to store the status string
// param: status_code: http reponse status code (200, 400, 404,...)
void get_reason(char dest[], int status_code);

void construct_head(char response[], char *content, int status_code, int content_type_code);

// response[]          a place to put the response constructed
// *content            content to be sent for GET
// status_code 		   200 or some.. defined in response.c
// content_type_code   defined in shared.h
void construct_response(char response[], char *content, int status_code, int content_type_code);

#endif //DC_RESPONSE_H
