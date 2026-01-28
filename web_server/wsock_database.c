// TODO: here I will open a new port, a new main, 
// a new endpoint for the frontend to interact with the database
// This file will be used for all interactions with the database
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
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

    struct sockaddr_storage their_addr; // marked as their_addr in wsock_server
    socklen_t client_addr_len;
    struct addrinfo hints, *res;
    int server_fd;
    int reuse_addr_flag = 1;

    // Define what we expect in addrinfo
    memset(&hints, 0, sizeof hints); // sets sizeof hints bytes of 0 to hints
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get the pointer to addrinfo (not actually needed bc of the *res above)
    const char *host = (argc > 1) ? argv[1] : NULL;
    int addr_info = getaddrinfo(host, MYPORT, &hints, &res);
    if (addr_info != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addr_info));
        exit(1);
    }
    printf("IP address for %s\n", argv[1]);

    // This whole bit is lowkey useless
    char ipstr[INET6_ADDRSTRLEN];

    inet_ntop(res->ai_family, argv[1], ipstr, sizeof ipstr);
    printf("Server IP address: %s\n", ipstr);

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        error("Failed to create socket\n");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_flag, sizeof(reuse_addr_flag)) == -1) {
        error("Failed to set socket options\n");
    }
    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        error("Unable to bind socket to port\n");
    }
    freeaddrinfo(res);
    
    printf("Starting WebSocket server on port %s\n", MYPORT);
    if (listen(server_fd, BACKLOG) == -1) {
        error("Failed to listen");
    }

    // We'll need a queue of clients, they don't all need to access the db at the same time
    // but their requests can't be ignored
    client_addr_len = sizeof(their_addr);

    int fd_count = 1;
    char buffer[BUFFER_SIZE];

    while (1) {

        // Check for new connections
        int client_fd = accept(server_fd, (struct sockaddr *)&their_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        int keepalive = 1;
        if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == -1) {
            perror("Failed to set SO_KEEPALIVE");
        }

        struct sockaddr_in *client_addr;
        client_addr = (struct sockaddr_in*)&their_addr;
        char client_ip[INET6_ADDRSTRLEN];
        inet_ntop(their_addr.ss_family, &client_addr->sin_addr, client_ip, sizeof(client_ip));
        printf("New connection from %s from socket %d\n", client_ip, client_fd);


        printf("Going to receive HTTP request on NJ code\n");
        parse_HTTP_requests(client_fd);
    }

    shutdown(server_fd, 2);
    printf("REACHED end of file\n");
    return 0;
}
