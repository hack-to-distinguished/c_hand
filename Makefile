# Compiler and flags
CC = gcc
# CFLAGS = -g -O1 -pthread -fsanitize=address -fno-omit-frame-pointer
CFLAGS = -g -O1 -Wall -Wextra -pthread -fsanitize=address -fno-omit-frame-pointer

# Threadpool HTTP server
THREADPOOL_SRCS = threaded_server_src/http.c threaded_server_src/threadpool.c threaded_server_src/threadpoolserver.c
THREADPOOL_BIN = threadpoolserver

# WebSocket server 
WSOCK_SERVER_SRC = web_server/wsock_server.c web_server/wsock_functions.c web_server/sha1.c database/storage/message_store.c
WSOCK_SERVER_BIN = wsock_server

# WebSocket database
WSOCK_DB_SRC = web_server/wsock_database.c threaded_server_src/http.c 
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
SDBMS_SRCS = \
	database/input_buffer.c \
	database/sdbms.c \
	database/tokenizer/token_list.c \
	database/tokenizer/scanner.c \
	database/parser/parser.c \
	database/parser/parser_utils.c \
	database/parser/parsing_functions/parse_sql_statement.c \
	database/parser/parsing_functions/parse_select_statement.c \
	database/parser/parsing_functions/parse_exit_statement.c \
	database/parser/parsing_functions/parse_delete_statement.c \
	database/parser/parsing_functions/parse_insert_statement.c \
	database/parser/parsing_functions/parse_update_statement.c \
	database/parser/parsing_functions/parse_select_list.c \
	database/parser/parsing_functions/parse_simple_expression.c \
	database/parser/parsing_functions/parse_term.c \
	database/parser/parsing_functions/parse_additive_operator.c \
	database/parser/parsing_functions/parse_factor.c \
	database/parser/parsing_functions/parse_multiplicative_operator.c \
	database/parser/parsing_functions/parse_qualified_identifier.c \
	database/parser/parsing_functions/parse_literal.c \
	database/parser/parsing_functions/parse_function_call.c \
	database/parser/parsing_functions/parse_arg_list.c \
	database/parser/parsing_functions/parse_table_list.c 
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
