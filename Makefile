# Compiler and flags
CC = gcc
CFLAGS = -g -O1 -Wall -Wextra -pthread -fsanitize=address -fno-omit-frame-pointer

# Threadpool HTTP server
THREADPOOL_SRCS = threaded_server_src/http.c threaded_server_src/threadpool.c threaded_server_src/threadpoolserver.c database/storage/message_store.c
THREADPOOL_BIN = threadpoolserver

# WebSocket server 
WSOCK_SERVER_SRC = web_server/wsock_server.c web_server/wsock_functions.c web_server/sha1.c database/storage/message_store.c threaded_server_src/http.c
WSOCK_SERVER_BIN = wsock_server

# WebSocket database
WSOCK_DB_SRC = web_server/wsock_database.c threaded_server_src/http.c database/storage/message_store.c
WSOCK_DB_BIN = wsock_database

# SSH server/client
SSH_SERVER_SRCS = ssh_server/server.c
SSH_SERVER_BIN = ssh_server/server
SSH_CLIENT_SRCS = ssh_server/client.c
SSH_CLIENT_BIN = ssh_server/client

# Simple client
CLIENT_SRCS = client.c
CLIENT_BIN = client

# Simple Database Management System
SDBMS_SRCS = database/input_buffer.c database/sdbms.c database/token_list.c database/scanner.c
SDBMS_BIN = sdbms


all: $(THREADPOOL_BIN) $(WSOCK_SERVER_BIN) $(SSH_SERVER_BIN) $(SSH_CLIENT_BIN) $(CLIENT_BIN) $(SDBMS_BIN) $(WSOCK_DB_BIN)

$(THREADPOOL_BIN): $(THREADPOOL_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(WSOCK_SERVER_BIN): $(WSOCK_SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(WSOCK_DB_BIN): $(WSOCK_DB_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(SSH_SERVER_BIN): $(SSH_SERVER_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(SSH_CLIENT_BIN): $(SSH_CLIENT_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_BIN): $(CLIENT_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

$(SDBMS_BIN): $(SDBMS_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(THREADPOOL_BIN) $(WSOCK_SERVER_BIN) $(SSH_SERVER_BIN) $(SSH_CLIENT_BIN) $(CLIENT_BIN) $(SDBMS_BIN) $(WSOCK_DB_BIN)

.PHONY: all clean
