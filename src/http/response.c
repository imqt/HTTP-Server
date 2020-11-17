//
// Created by wuviv on 2020-11-04.
//

#include "response.h"

char * get_content( char file_name[]) {
    char * buffer = 0;
    long length;
    FILE * f = fopen(file_name, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
    }
    if (buffer) {
        fread(buffer, 1, length, f);
    }
    fclose(f);
    if (buffer) {
        return buffer;
    } else {}
}

void get_reason(char dest[], int status_code) {
    switch(status_code) {
        case 200:
            strcpy(dest, "200 OK\n\0"); break;
        case 400: 
            strcpy(dest, "400 Bad Request\n\0"); break;
        case 404:  // html not found
            strcpy(dest, "404 Not Found\n\0"); break; 
        case 501:  // Requested method not implemented (POST, PUT, TRACE)
            strcpy(dest, "501 Not Implemented\n\0"); break;
        default:
            strcpy(dest, ""); break;
    }
}

void construct_response(char response[], char *content, int status_code) {
    char httpver[] = "HTTP/1.0 ";                       //  9
    char status_reason[100];
        get_reason(status_reason, status_code);
    char content_type[] = "Content-Type: text/html\n";  // 24
    char content_length[] = "Content-Length: ";         // 16
    char len[9] = "";
        snprintf(len, sizeof(len), "%ld", strlen(content));

    strcat(response, httpver);
    strcat(response, status_reason);
    strcat(response, content_type);
    strcat(response, content_length); 
    strcat(response, len);  //TODO: int to string
    strcat(response, "\n\n");
    strcat(response, content);

    free(content);
}
