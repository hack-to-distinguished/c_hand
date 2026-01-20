// TODO: here I will open a new port, a new main, 
// a new endpoint for the frontend to interact with the database
// This file will be used for all interactions with the database
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "../database/storage/message_store.h"
#include "../threaded_server_src/http.h"
#include "wsock_functions.h"

#define MYPORT "8082"
#define BACKLOG 20
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 20

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    
    struct addrinfo hints;
    struct addrinfo *res;

    // Define what we expect in addrinfo
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get the pointer to addrinfo (not actually needed bc of the *res above)
    // getaddrinfo();




    return 0;
}
