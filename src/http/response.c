//
// Created by wuviv on 2020-11-04.
//

#include "response.h"

char textPlain[] = "text/plain";
char textHTML[]   = "text/html";
char imgPNG[]     = "image/png";
char imgWEBP[]     = "image/webp";
char audioMPEG[]  = "audio/mpeg";
char favicon[]  = "image/webp";

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
    return NULL;
}

void respond(int cfd, char * file_name, int content_type_code) {
    char response[BUF_SIZE] = "";
    if (realpath(file_name, NULL)!= NULL) {
        // Constuct a reponse:
        construct_response(response, get_content(file_name), 200, content_type_code);
    } else {
        // TODO: differentiate between handling html not found and videos/images not found
        construct_response(response, get_content("../../rsc/404.html"), 404, content_type_code);
    }
    dc_write(STDOUT_FILENO, "\n//////////////////////AFter construct_response\n", 50);
    // Send response to client
    dc_write(cfd, response, strlen(response));
    // Print to server's terminal
    dc_write(STDOUT_FILENO, response, strlen(response));
    dc_write(STDOUT_FILENO, "\n//////////////////////////////AFter responding\n", 50);
}

void get_reason(char dest[], int status_code) {
    switch(status_code) {
        case 200:
            strcpy(dest, "200 OK"); break;
        case 400: 
            strcpy(dest, "400 Bad Request"); break;
        case 404:  // html not found
            strcpy(dest, "404 Not Found"); break; 
        case 501:  // Requested method not implemented (POST, PUT, TRACE)
            strcpy(dest, "501 Not Implemented"); break;
        default:
            strcpy(dest, ""); break;
    }
}

void construct_head(char response[], char *content, int status_code, int content_type_code) {
    char httpver[] = "HTTP/1.0 ";
    char status_reason[100];
        get_reason(status_reason, status_code);
    char content_type[] = "\r\nContent-Type: ";
    char content_length[] = "\r\nContent-Length: ";
    char len[9] = "";
        snprintf(len, sizeof(len), "%ld", strlen(content));

    strcat(response, httpver);
    strcat(response, status_reason);
    strcat(response, content_type);

    switch(content_type_code) {
        case 0:
            strcat(response, textHTML); break;
        case 1: 
            strcat(response, imgWEBP); break;
        case 2:
            strcat(response, audioMPEG); break;
        case 3:
            strcat(response, favicon); break;
        default:
            strcat(response, textPlain); break;
    }

    strcat(response, content_length);
    strcat(response, len);  //TODO: int to string
    strcat(response, "\r\n\r\n");
}

void construct_response(char response[], char *content, int status_code, int content_type_code) {
    construct_head(response, content, status_code, content_type_code);
    strcat(response, content);

    free(content);
}

// Audio    audio/mpeg
//          audio/mpeg3
//          audio/x-ms-wma
//          audio/vnd.rn-realaudio
//          audio/x-wav

// Image    image/gif
//          image/jpeg
//          image/png
//          image/tiff
//          image/vnd.microsoft.icon
//          image/x-icon
//          image/vnd.djvu
//          image/svg+xml

// Text     text/css  so apparently most text stuffs can just be sent as text/html
//          text/csv
//          text/html
//          text/javascript (obsolete)
//          text/plain
//          text/xml

