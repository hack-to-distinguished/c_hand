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

typedef struct {
    int fd;
    bool is_websocket;
    char ip[INET6_ADDRSTRLEN];
} client_t;

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


    client_t clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = -1;
        clients[i].is_websocket = false;
    }

    // We'll need a queue of clients, they don't all need to access the db at the same time
    // but their requests can't be ignored
    client_addr_len = sizeof(their_addr);

    struct pollfd pfds[MAX_CLIENTS + 1]; // +1 for the server socket
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN;

    int fd_count = 1;
    char buffer[BUFFER_SIZE];

    while (1) {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1) {
            error("Poll error");
        }

        // Check for new connections
        if (pfds[0].revents & POLLIN) {
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

            // Check if we can accept more clients
            if (fd_count >= MAX_CLIENTS + 1) {
                printf("Server full, putting client in a queue for next loop\n");
                const char *msg = "Pending";
                send(client_fd, msg, strlen(buffer), 0);
                // TODO: remove the close and add the user to a queue
                shutdown(client_fd, 1);
                continue;
            }

            // Read the initial HTTP request
            printf("Going to receive HTTP request on NJ code\n");
            char* bytes_read = receive_HTTP_request(client_fd);
            printf("Received http request: %s\n", bytes_read);


            // TODO: add logic here to act based on the HTTP request from earlier
            const char *sec_key  = "x"; // change this to get an actuall proper security key
            if (sec_key) {
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i].fd == -1) {
                        clients[i].fd = client_fd;
                        clients[i].is_websocket = true;
                        strncpy(clients[i].ip, client_ip, sizeof(clients[i].ip));
                        break;
                    }
                }


                pfds[fd_count].fd = client_fd;
                pfds[fd_count].events = POLLIN;
                pfds[fd_count].revents = 0; // Ensures that new conn starts with a clean slate
                fd_count++;
            } else {
                printf("Non-WebSocket request from %s, sending HTTP response\n", client_ip);
                ws_close_websocket_http_response(client_fd, "This server only accepts WebSocket connections\n");
                shutdown(client_fd, 2);
                continue;
            }
        }

        for (int i = 1; i < fd_count; i++) {
            if (pfds[i].revents & POLLIN) {
                int client_sock = pfds[i].fd;

                int client_idx = -1;
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].fd == client_sock) {
                        client_idx = j;
                        break;
                    }
                }
            }

            // Check for errors or disconnects
            if (pfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                int client_sock = pfds[i].fd;

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].fd == client_sock) {
                        printf("Client %s on socket %d has error or disconnected\n", clients[j].ip, client_sock);
                        shutdown(client_sock, 2);
                        clients[j].fd = -1;
                        clients[j].is_websocket = false;
                        break;
                    }
                }

                pfds[i] = pfds[fd_count - 1];
                fd_count--;
                i--;
            }
        }



    }

    shutdown(server_fd, 2);
    printf("REACHED end of file\n");
    return 0;
}
