#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sha1.h"
#include "../threaded_server_src/http.h"
#include "wsock_functions.h"

#define BUFFER_SIZE 2048

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
size_t ws_base64_encode(const unsigned char *in, size_t in_len, char *out, size_t out_size) {
    size_t i = 0, o = 0;
    while (i < in_len) {
        unsigned char a3[3] = {0,0,0};
        int j;
        for (j = 0; j < 3 && i < in_len; j++, i++)
            a3[j] = in[i];

        if (o + 4 >= out_size)  // prevent overflow
            return 0;

        out[o++] = b64_table[(a3[0] & 0xfc) >> 2];
        out[o++] = b64_table[((a3[0] & 0x03) << 4) | ((a3[1] & 0xf0) >> 4)];
        out[o++] = (j > 1) ? b64_table[((a3[1] & 0x0f) << 2) | ((a3[2] & 0xc0) >> 6)] : '=';
        out[o++] = (j > 2) ? b64_table[a3[2] & 0x3f] : '=';
    }
    if (o < out_size)
        out[o] = '\0';
    return o;  // returns number of bytes written (not counting null terminator)
}

ssize_t ws_recv_frame(int sock, char *out, size_t max_len) {
    unsigned char hdr[2];
    ssize_t r = recv(sock, hdr, 2, 0);
    if (r == 0) return -1; // EOF error (disconnected)
    if (r < 0) return -1; // TCP Error

    // int fin = hdr[0] & 0x80;
    int opcode = hdr[0] & 0x0F;
    int masked = hdr[1] & 0x80;
    size_t payload_len = hdr[1] & 0x7F;

    // Handle close frame
    if (opcode == 8) {
        printf("\nReceived close frame from socket %d\n\n", sock);
        // Send close frame back
        unsigned char close_frame[2] = {0x88, 0x00}; // Close frame with no payload
        send(sock, close_frame, 2, 0);
        return -1;
    }

    if (opcode == 9) {
        printf("Received ping from socket %d, sending pong\n", sock);
        unsigned char pong[2] = {0x8A, 0x00}; // Pong frame
        send(sock, pong, 2, 0);
        return 0; // Don't pass to application
    }

    if (payload_len == 126) {
        unsigned char ext[2];
        if (recv(sock, ext, 2, 0) <= 0) return -1;
        payload_len = (ext[0] << 8) | ext[1];
    } else if (payload_len == 127) {
        unsigned char ext[8];
        if (recv(sock, ext, 8, 0) <= 0) return -1;
        // For simplicity, only support <= 2^32
        payload_len = 0;
        for (int i = 4; i < 8; i++) // Use lower 4 bytes only
            payload_len = (payload_len << 8) | ext[i];
    }

    if (payload_len == 0) return 0;
    if (payload_len > max_len - 1) { // Leave room for null terminator
        printf("Message too large (payload_len=%zu, max=%zu)\n", payload_len, max_len - 1);
        return -1;
    }

    unsigned char mask[4] = {0};
    if (masked) {
        if (recv(sock, mask, 4, 0) <= 0) return -1;
    }

    size_t bytes_read = 0;
    while (bytes_read < payload_len) {
        ssize_t result = recv(sock, out + bytes_read, payload_len - bytes_read, 0);
        if (result <= 0) return -1;
        bytes_read += result;
    }

    if (masked) {
        for (size_t i = 0; i < payload_len; i++) {
            out[i] ^= mask[i % 4];
        }
    }

    out[payload_len] = '\0';
    return payload_len;
}

void ws_send_frame(int sock, const char *msg) {
    size_t len = strlen(msg);

    printf("Sending to client %d: %s\n", sock, msg);

    // Prepare header (assuming len < 65536)
    unsigned char header[10] = {0};
    header[0] = 0x81; // Text frame, FIN bit set

    if (len < 126) {
        header[1] = len;
        if (send(sock, header, 2, 0) < 2) {
            perror("Failed to send WebSocket header");
            return;
        }
    } else if (len < 65536) {
        header[1] = 126;
        header[2] = (len >> 8) & 0xFF;
        header[3] = len & 0xFF;
        if (send(sock, header, 4, 0) < 4) {
            perror("Failed to send WebSocket header");
            return;
        }
    } else {
        header[1] = 127;
        // Set length bytes, assuming len is less than 2^32
        for (int i = 0; i < 8; i++) {
            header[9-i] = (len >> (i * 8)) & 0xFF;
        }
        if (send(sock, header, 10, 0) < 10) {
            perror("Failed to send WebSocket header");
            return;
        }
    }

    if (send(sock, msg, len, 0) < (ssize_t)len) {
        perror("Failed to send WebSocket payload");
    }
}

void ws_close_websocket_http_response(int sock, const char *body) {
    char response[BUFFER_SIZE];
    char escaped[BUFFER_SIZE];

    // JSON escaping
    size_t j = 0;
    for (size_t i = 0; body[i] && j < BUFFER_SIZE - 2; i++) {
        if (body[i] == '"' || body[i] == '\\') {
            escaped[j++] = '\\';
        }
        escaped[j++] = body[i];
    }
    escaped[j] = '\0';

    int body_len = snprintf(NULL, 0, "{\"message\": \"%s\"}", escaped);

    snprintf(response, sizeof(response),
         "HTTP/1.1 200 OK\r\n"
         "Content-Type: application/json\r\n"
         "Access-Control-Allow-Origin: *\r\n"
         "Content-Length: %d\r\n"
         "Connection: close\r\n"
         "\r\n"
         "{\"message\": \"%s\"}",
         body_len, escaped);
    write(sock, response, strlen(response));
}

void ws_send_websocket_response(int sock, const char *accept_key) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
         "HTTP/1.1 101 Switching Protocols\r\n"
         "Upgrade: websocket\r\n"
         "Connection: Upgrade\r\n"
         "Sec-WebSocket-Accept: %s\r\n\r\n",
         accept_key);
    write(sock, response, strlen(response));
}

const char *ws_parse_websocket_http(const char *http_header) {
    if (!http_header || http_header[0] == '\0') {
        printf("Empty HTTP header\n");
        return NULL;
    }

    if (strstr(http_header, "Upgrade: websocket") == NULL) {
        printf("Not a WebSocket upgrade request\n");
        return NULL;
    }

    char *key = NULL;
    char *header_copy = strdup(http_header);
    if (!header_copy) {
        perror("Failed to allocate memory for header copy");
        return NULL;
    }

    const char *needle = "Sec-WebSocket-Key:";
    char *line = strtok(header_copy, "\r\n");
    while (line != NULL) {
        if (strncmp(line, needle, strlen(needle)) == 0) {
            const char *value_start = line + strlen(needle);
            while (*value_start == ' ') value_start++;
            key = strdup(value_start);
            break;
        }
        line = strtok(NULL, "\r\n");
    }
    free(header_copy);

    if (key == NULL) {
        printf("Error: Sec-WebSocket-Key not found.\n");
        return NULL;
    }

    const char *magic_str = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    char concat_str[256];
    snprintf(concat_str, sizeof(concat_str), "%s%s", key, magic_str);
    free(key);

    unsigned char digest[20];
    SHA1_CTX ctx;
    SHA1Init(&ctx);
    SHA1Update(&ctx, (const unsigned char*)concat_str, strlen(concat_str));
    SHA1Final(digest, &ctx);

    static char accept_key[32]; // Base64 of 20 bytes is 28 chars + padding + null
    ws_base64_encode(digest, 20, accept_key, sizeof(accept_key));
    printf("Sec-WebSocket-Accept: %s\n", accept_key);
    return accept_key;
}

typedef struct {
    int fd;
    bool is_websocket;
    char ip[INET6_ADDRSTRLEN];
} client_t;