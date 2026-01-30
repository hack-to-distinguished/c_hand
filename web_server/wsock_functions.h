#include <netinet/in.h>

size_t ws_base64_encode(const unsigned char *in, size_t in_len, char *out, size_t out_size);
ssize_t ws_recv_frame(int sock, char *out, size_t max_len);
void ws_send_frame(int sock, const char *msg);
void ws_close_websocket_http_response(int sock, const char *body);
void ws_send_websocket_response(int sock, const char *accept_key);
const char *ws_parse_websocket_http(const char *http_header);