#include "request.h"

int parse_request(char request[]) {
    char GET_R[] = "GET";
    for (int i = 0; i < 3; i++)
        if (GET_R[i] != request[i])
            return 0; // unsupported request
    return 1; // GET request
}
