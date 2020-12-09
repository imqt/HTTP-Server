#include "request.h"

#define GETC 5   // "GET /"
#define HEADC 6  // "HEAD /"

char GET_R[] = "GET";   
char HEAD_R[] = "HEAD"; 

int parse_request(char request[], char file_name[], int request_len) {
    // Print client request to stderr
	fprintf(stderr, "== Request:\n%s\n", request);
	int request_code = get_method(request);
	if (request_code == 0) return 0;
	int start_index = request_code; // 5 or 6 (get or head)
	int end_index = start_index;
	while (request[end_index++] != ' ');
	end_index--;
	strncat(file_name, (request+start_index), (end_index-start_index));
	return request_code;
}

int get_method(char request[]) {
    return (strncmp(request, GET_R,  strlen(GET_R))  == 0) ? GETC
         : (strncmp(request, HEAD_R, strlen(HEAD_R)) == 0) ? HEADC
    								  	                   : 0;
} 
