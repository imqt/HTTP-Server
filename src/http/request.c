#include "request.h"

#define GETC 5   // "GET /"
#define HEADC 6  // "HEAD /"

char GET_R[] = "GET";   
char HEAD_R[] = "HEAD"; 
char png[] = "png";
char webp[] = "webp";
char mp3[] = "mp3";
char ico[] = "ico";

int find_last_dot_before_space(char request[], int start_index);


int parse_request(char request[], char file_name[], int*content_type_code, int request_len) {
        // Print client request to stdout
    dc_write(STDOUT_FILENO, "\n///////////////////////////////////New Request\n", 50);
    dc_write(STDOUT_FILENO, request, request_len);  

	int request_code = get_method(request);
	if (request_code == 0) return 0;
	int start_index = request_code; // 5 or 6 (get or head)
	int end_index = start_index;
	while (request[++end_index] != ' ');

	strncat(file_name, (request+start_index), (end_index-start_index));

	fprintf(stderr, "Requested file: ");
	for (int i = start_index; i < (end_index); i++)
		fprintf(stderr, "%c.", request[i]);
	// fprintf(stderr, "\n");
	// fprintf(stderr, "\nstart  %d", start_index);
	// fprintf(stderr, "\nlength %d", end_index-start_index);
	fprintf(stderr, "\n");

	// What'd be a smarter way to parse for file type...?
	start_index = find_last_dot_before_space(request, start_index) + 1;
	end_index = start_index;
	while (request[++end_index] != ' ');
	if      (strncmp(webp, (request+start_index), (end_index-start_index)) == 0)
		*content_type_code = 1;
	else if (strncmp(mp3, (request+start_index), (end_index-start_index))  == 0)
		*content_type_code = 2;
	else if (strncmp(ico, (request+start_index), (end_index-start_index))  == 0)
		*content_type_code = 3;
	else
		*content_type_code = 0;

	fprintf(stderr, "Requested file type: ");
	for (int i = start_index; i < (end_index); i++)
		fprintf(stderr, "%c.", request[i]);
	fprintf(stderr, "\nContent-type code: %d", *content_type_code);
    
    dc_write(STDOUT_FILENO, "\n///////////////////////////After parse_request\n", 50);
	dc_write(STDOUT_FILENO, file_name, strlen(file_name));
	return request_code;
}

int find_last_dot_before_space(char request[], int start_index) {
	int last = 0;
	for (int index = start_index;  request[index++] != ' '; last = (request[index] == '.') ? index : last);
	return last;
}


int get_method(char request[]) {
	int result = 0;

    return (strncmp(request, GET_R,  strlen(GET_R))  == 0) ? GETC
         : (strncmp(request, HEAD_R, strlen(HEAD_R)) == 0) ? HEADC
    								  	                   : 0;
} 
