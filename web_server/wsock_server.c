#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "../database/storage/message_store.h"
#include "../threaded_server_src/http.h"
#include "wsock_functions.h"

#define MYPORT "8081"
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
    struct addrinfo hints, *res;
    struct sockaddr_storage their_addr;
    socklen_t their_addrlen = sizeof(their_addr);
    int server_fd;
    int reuse_addr_flag = 1;

    client_t clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = -1;
        clients[i].is_websocket = false;
    }

    memset(&hints, 0, sizeof hints); // will just copy 0s
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    const char *host = (argc > 1) ? argv[1] : NULL;

    int addr_info = getaddrinfo(host, MYPORT, &hints, &res);
    if (addr_info != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addr_info));
        exit(1);
    }

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        error("Failed to create socket");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_flag, sizeof(reuse_addr_flag)) == -1) {
        error("Failed to set socket options");
    }

    int bind_conn = bind(server_fd, res->ai_addr, res->ai_addrlen);
    if (bind_conn == -1) {
        error("Unable to bind to address");
    }

    freeaddrinfo(res);

    printf("Starting WebSocket server on port %s\n", MYPORT);

    if (listen(server_fd, BACKLOG) == -1) {
        error("Failed to listen");
    }

    struct pollfd pfds[MAX_CLIENTS + 1]; // +1 for the server socket
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN;

    int fd_count = 1;
    char buffer[BUFFER_SIZE];

    fms[0].ID = 999999;
    time_t now = time(NULL);
    int* latest_entry_ptr = &fms[0].ID;
    latest_entry_ptr = ms_point_to_last_entry(fms);

    while (1) {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1) {
            error("Poll error");
        }

        // Check for new connections
        if (pfds[0].revents & POLLIN) {
            struct sockaddr_in *client_addr;
            int client_fd = accept(server_fd, (struct sockaddr *)&their_addr, &their_addrlen);
            if (client_fd == -1) {
                perror("Accept failed");
                continue;
            }

            int keepalive = 1;
            if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == -1) {
                perror("Failed to set SO_KEEPALIVE");
            }

#ifdef TCP_KEEPIDLE
            int keepidle = 10;    // Start probing after 10 seconds idle
            if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) == -1) {
                perror("Failed to set TCP_KEEPIDLE");
            }
#elif defined(TCP_KEEPALIVE)  // macOS uses TCP_KEEPALIVE instead
            int keepidle = 10;
            if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPALIVE, &keepidle, sizeof(keepidle)) == -1) {
                perror("Failed to set TCP_KEEPALIVE");
            }
#endif

#ifdef TCP_KEEPINTVL
            int keepintvl = 5;    // Probe every 5 seconds
            if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) == -1) {
                perror("Failed to set TCP_KEEPINTVL");
            }
#endif

#ifdef TCP_KEEPCNT
            int keepcnt = 3;      // Drop after 3 failed probes
            if (setsockopt(client_fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) == -1) {
                perror("Failed to set TCP_KEEPCNT");
            }
#endif

            client_addr = (struct sockaddr_in*)&their_addr;
            char client_ip[INET6_ADDRSTRLEN];
            inet_ntop(their_addr.ss_family, &client_addr->sin_addr, client_ip, sizeof(client_ip));

            printf("New connection from %s on socket %d\n", client_ip, client_fd);

            // Check if we can accept more clients
            if (fd_count >= MAX_CLIENTS + 1) {
                printf("Server full, rejecting connection from %s\n", client_ip);
                const char *msg = "Server full";
                ws_close_websocket_http_response(client_fd, msg);
                close(client_fd);
                continue;
            }

            // Read the initial HTTP request
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_read <= 0) {
                perror("Receive failed");
                close(client_fd);
                continue;
            }
            buffer[bytes_read] = '\0';

            const char *accept_key = ws_parse_websocket_http(buffer);
            if (accept_key) {
                ws_send_websocket_response(client_fd, accept_key);

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

                printf("WebSocket connection established with %s on socket %d\n", client_ip, client_fd);

                // TODO:
                // This is where the databse read should happend
                // because at this point, the connection is valid

                // TODO: I can create another if here or within the else
                // Search what kind of request it is and produce another outcome
            } else {
                printf("Non-WebSocket request from %s, sending HTTP response\n", client_ip);
                ws_close_websocket_http_response(client_fd, "This server only accepts WebSocket connections\n");
                close(client_fd);
                continue;
            }
        }

        // Check existing connections for data
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

                // FIX: The handling of invalid/Closed sockets is wrong
                if (client_idx == -1 || !clients[client_idx].is_websocket) {
                    printf("Invalid client socket %d, closing\n", client_sock);
                    close(client_sock);
                    pfds[i] = pfds[fd_count - 1];
                    fd_count--;
                    i--;
                    continue;
                }

                memset(buffer, 0, BUFFER_SIZE);
                ssize_t bytes_recv = ws_recv_frame(client_sock, buffer, BUFFER_SIZE - 1);

                // FIX: The handling of invalid/Closed sockets is wrong
                if (bytes_recv < 0) {
                    printf("Client %s on socket %d disconnected\n", clients[client_idx].ip, client_sock);
                    close(client_sock);

                    clients[client_idx].fd = -1;
                    clients[client_idx].is_websocket = false;

                    pfds[i] = pfds[fd_count - 1];
                    fd_count--;
                    i--;
                    continue;
                }

                if (bytes_recv == 0) {
                    // Control for ping/pong
                    continue;
                }

                printf("Received from %s (%d): %s\n", clients[client_idx].ip, client_sock, buffer);
                ms_add_message(clients[client_idx].ip, "all", buffer, &now, &now, fms, &latest_entry_ptr);

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].fd != -1 && clients[j].is_websocket) {
                        ws_send_frame(clients[j].fd, buffer);
                    }
                }
            }

            // Check for errors or disconnects
            if (pfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                int client_sock = pfds[i].fd;

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (clients[j].fd == client_sock) {
                        printf("Client %s on socket %d has error or disconnected\n", clients[j].ip, client_sock);
                        close(client_sock);
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

    close(server_fd);
    return 0;
}
