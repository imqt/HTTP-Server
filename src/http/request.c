#include "request.h"


int parse_request(char request[], char file_name[]) {
	int request_code = get_method(request);
	if (request_code == 0) return 0;
	int start_index = (request_code == 1) ? 5 : 6;
	int end_index = start_index;
	while (request[end_index++] != ' ');
	strncat(file_name, (request+request_code+1), (end_index-start_index));
	return request_code;
}


int get_method(char request[]) {
	int result = 0;
	char GET_R[] = "GET";   // 3
	char HEAD_R[] = "HEAD"; // 4
    return (strncmp(request, GET_R,  3)) ? 3 
          : strncmp(request, HEAD_R, 4)  ? 4 
    								  	 : 0;
} 
